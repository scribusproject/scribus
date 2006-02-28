<!DOCTYPE TS><TS>
<context>
    <name></name>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Devuelve una lista que contiene los nombres de todos los colores definidos 
en el documento. Si no hay abierto ningún documento, devulve una lista de 
los colores del documento por defecto.
</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Muestra el diálogo de &quot;Nuevo Documento&quot;. Crea un nuevo documento si 
el usuario presiona aceptar. No lo crea si presiona cancelar. Devuelve true 
si se crea un nuevo documento.</translation>
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

Crea un nuevo documento y devuelve true si tiene éxito. Los parámetros tienen 
el siguiente significado:

    size = Un par (ancho, alto) que describe el tamaño del documento. Se 
    pueden usar constantes predefinidas llamadas PAPER_&lt;tipo_de_papel&gt; 
    p. ej. PAPER_A4, etc.

    margins = Un vector (izquierda, derecha, arriba, abajo) que describe 
    los márgenes del documento

    orientation = la orientación de la página - constantes PORTRAIT (vertical), 
    LANDSCAPE (apasaido)

   firstPageNumber = es el número de la primera página del documento 
    usado para la numeración de páginas. Aunque normalmente será 1, es útil 
    para tener números mayores si se crea un documento en varias partes.

    unit: este valor establece las unidades de medida usadas por el 
    documento. Usa una constante predefinida para esto, una de: 
    UNIT_INCHES (pulgadas), UNIT_MILLIMETERS (milímetros), 
    UNIT_PICAS (picas), UNIT_POINTS (puntos).

    facingPages = FACINGPAGES (páginas enfrentadas), 
    NOFACINGPAGES (páginas no enfrentadas)

    firstSideLeft = FIRSTPAGELEFT (primera página a la izquierda), 
    FIRSTPAGERIGHT (primera página a la derecha)

Los valores para la anchura, altura y los márgenes están expresados en la unidad 
dada para el documento. Las constantes PAPER_* están expresadas en puntos. Si 
tu documento no está en puntos, asegurate de tenerlo en cuenta. 

ejemplo: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS, 
                FACINGPAGES, FIRSTPAGERIGHT)
</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Devuleve el nombre del color de relleno del objeto con nombre&quot;name&quot;. 
Si no se pasa un nombre se usa el objeto que esté seleccionado.
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

Mueve el objeto de nombre &quot;name&quot; dx y dy unidades, relativo a su posición 
actual. Las distancias se deben expresar en las unidades actuales del 
documento (ver constantes UNIT). Si no se pasa un nombre se usa el objeto 
que esté seleccionado. Si el objeto &quot;name&quot; pertenece a un grupo, se 
mueve el grupo entero.
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Desactiva el redibujado de la página si bool = False, de otro modo, se activa 
el redibujado. Este cambio permanecerá incluso después de que el script 
termine, así que asegurate de llamar a setRedraw(True) en una sentencia 
finally: en el nivel superior del script.
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
        <translation>createRect(x, y, width, height, [&quot;name&quot;]) -&gt; string

Crea un nuevo rectángulo en la página actual y devuelve su nombre. Las 
coordenadas x, y, anchura y altura se expresan en las unidades actuales 
del documento (ver constantes UNIT). &quot;name&quot; debería ser un identificador 
único para el objeto porque el objeto se referenciará con ese nombre 
después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando.
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
        <translation type="obsolete">newPage(where [,&quot;template&quot;])

Crea una nueva página. Si &quot;where&quot; es -1 la página se añade al documento, 
de otro modo la página se inserta antes de la página &quot;where&quot;. Los números 
de página se cuentan de 1 en adelante, sin importar cuál es el número que se 
muestra en la primera página del documento. El parámetro opcional &quot;template&quot; 
especifica el nombre de la plantilla para la nueva página.

Puede lanzar la excepción IndexError si el número de página está fuera de rango.
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Establece el degradado de relleno del objeto con nombre &quot;name&quot; como 
tipo &quot;type&quot;. Las descripciones de color son las mismas que para setFillColor() 
y setFillShade(). Ver las constantes FILL_&lt;tipo&gt; para los tipos disponibles.
</translation>
    </message>
    <message>
        <source>getFontSize([&quot;name&quot;]) -&gt; float

Returns the font size in points for the text frame &quot;name&quot;. If this text
frame has some text selected the value assigned to the first character of
the selection is returned.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFontSize([&quot;name&quot;]) -&gt; float

Devuelve el tamaño de fuente en puntos del marco de texto con nombre 
&quot;name&quot;. Si el marco de texto tiene texto seleccionado, devuelve el del 
primer caracter de la selección. 
Si no se le pasa un nombre se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Escribe la cadena &quot;string&quot; en la barra de mensaje (barra de estado) de Scribus. 
El texto debe estar codificado en UTF8 o &apos;unicode&apos; (recomendado).
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
        <translation type="obsolete">register_macro_callable(name, callable, accel=&apos;&apos;)

Crea una macro llamada &quot;name&quot; con el objeto ya existente llamado &quot;callable&quot;.
&quot;callable&quot; no debe requerir argumentos cuando se llame (puede tener
argumentos opcionales, pero no se le pasará ninguno). 
La cadena &quot;accel&quot; se usará como acceso rápido para la macro, si se
proporciona.
Si &quot;callable&quot; es una clase se rechazará. Se admiten funciones y métodos
bound, al igual que instancias de clases que proporcionen un método
__call__ sin argumentos. No hay ningún problema en registrar el mismo
&quot;callable&quot; más de una vez, ni en registrar múltiples métodos bound de una
única instancia.
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>importSVG(&quot;string&quot;)

The &quot;string&quot; must be a valid filename for a SVG image. The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
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

Devuelve un vector (C, M, Y, K) que tiene las cuatro componentes de 
color del color &quot;name&quot; del documento actual. Si no hay un documento 
abierto, usa los colores del documento por defecto. 

Puede lanzar la excepción NotFoundError si no se encuentra el color indicado. 
Puede lanzar la excpeción ValueError si se especifica un nombre no válido.
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
        <translation>changeColor(&quot;name&quot;, c, m, y, k)

Cambia el color &quot;name&quot; al valor CMYK especificado. El valor del color 
se define mediante cuatro componentes: c = cian, m = magenta, 
y = amarillo y k = negro. Las componentes deberían estar en el 
rango 0 a 255. 

Puede lanzar la excepción NotFoundError si no se encuentra el color indicado. 
Puede lanzar la excpeción ValueError si se especifica un nombre no válido.
</translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation type="obsolete">defineColor(&quot;name&quot;, c, m, y, k)

Define un nuevo color llamado &quot;name&quot;. El valor del color se define mediante 
cuatro componentes: c = cian, m = magenta, y = amarillo y k = negro. 
Las componentes deberían estar en el rango 0 a 255. 

Puede lanzar la excpeción ValueError si se especifica un nombre no válido.
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
        <translation>deleteColor(&quot;name&quot;, &quot;replace&quot;)

Borra el color &quot;name&quot;. Cada aparición de ese color se reemplaza 
con el color &quot;replace&quot;. Si no se especifica, se reemplaza por el 
color &quot;Ninguno&quot; - transparente.

Si no hay ningún documento abierto deleteColor funciona sobre los 
colores del documento por defecto. En ese caso, aunque se indique,
&quot;replace&quot; no tiene efecto.

Puede lanzar la excepción NotFoundError si no se encuentra el color indicado. 
Puede lanzar la excpeción ValueError si se especifica un nombre no válido.
</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Cada aparición del color &quot;name&quot; se reemplaza por el color &quot;replace&quot;.

Puede lanzar la excepción NotFoundError si no se encuentra el color indicado. 
Puede lanzar la excpeción ValueError si se especifica un nombre no válido.
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
        <translation type="obsolete">fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; string con nombre de archivo

Muestro un diálogo de Abrir Archivo con el título &quot;caption&quot;. Los archivos se 
filtran por la cadena &quot;filter&quot;. Se puede suministrar un nombre de archivo o 
ruta por defecto (defaultname), si no se quiere usar se debe dejar vacío. 
Si haspreview es True muestra una pequeña previsualización. Cuando el 
parámetro issave es True el diálogo se comporta como un diálogo de 
&quot;Guardar Como&quot;, de modo se comporta como un diálogo de &quot;Abrir Archivo&quot;. 
Ambos parámetros son False por defecto. 

El filtro, si se especifica, tiene la forma &apos;comentario (*.tipo *.tipo2 ...)&apos;. 
Por ejemplo &apos;Imágenes (*.png *.xpm *.jpg)&apos;.

Mirar la Documentación de Qt de QFileDialog para detalles de los filtros. 

Ejemplo: fileDialog(&apos;Abrir entrada&apos;, &apos;Archivos CSV (*.csv)&apos;) 
Ejemplo: fileDialog(&apos;Guardar informe&apos;, defaultname=&apos;report.txt&apos;, issave=True)
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
        <translation>messageBox(&quot;caption&quot;, &quot;message&quot;,
    icon=ICON_NONE, button1=BUTTON_OK|BUTTONOPT_DEFAULT,
    button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; integer

Muestra un cuadro de mensaje con el título &quot;caption&quot;, el mensaje &quot;message&quot;, 
un icono &quot;icon&quot; y hasta 3 botones. Por defecto no se usa ningún icono y sólo 
hay un botón, Aceptar. Sólo son obligatorios los parámetros caption y message, 
sin embargo se recomienda definir un icono y el botón o botones apropiados. 
El mensaje puede contener etiquetas simples similares a las de HTML.

Devuelve el número del botón que presionó el usuario. Los números de los 
botones empiezan en 1. 

Para los parámetros del iconoy de los botones hay constantes predefinidas 
disponibles con los mismos nombres que en la Documentación de Qt. Son las 
constantes BUTTON_* e ICON_* definidas en el módulo. Hay también otras 
dos constantes extra que pueden ser combinadas con un OR binario con las 
constantes de los botones: 
    BUTTONOPT_DEFAULT   Presionar intro presiona este botón.
    BUTTONOPT_ESCAPE    Presionar escape presiona este botón.

Ejemplos de uso: 
result = messageBox(&apos;Error en Script&apos;,
                    &apos;Este scrpt sólo funciona cuando tienes un marco de texto seleccionado.&apos;,
                    ICON_ERROR)
result = messageBox(&apos;Monkeys!&apos;, &apos;Something went ook! &lt;i&gt;Was it a monkey?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Botones definidos:
BUTTON_NONE (ninguno), BUTTON_ABORT (abortar), BUTTON_CANCEL (cancelar), 
BUTTON_IGNORE (ignorar), BUTTON_NO (no), BUTTON_NOALL (no a todo), 
BUTTON_OK (aceptar), BUTTON_RETRY (reintentar), BUTTON_YES (sí), 
BUTTON_YESALL (sí a todo)

Constantes de iconos:
ICON_NONE (ninguno), ICON_INFORMATION (información), ICON_WARNING (aviso), 
ICON_CRITICAL. (crítico)
</translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; string

Muestra el diálogo de &apos;Introducir una cadena&apos; y devuelve su valor 
como un string. 
Parámetros: título de la ventana, texto de la ventana y un valor 
por defecto opcional. 

Ejemplo: valueDialog(&apos;título&apos;, &apos;texto de la ventana&apos;, &apos;opcional&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Cierra el documento actual sin preguntar para guardar.

Puede lanzar la excepción NoDocOpenError si no hay ningún 
documento que cerrar.
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool

Devuelve true si hay un documento abierto.</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Abre el documento &quot;name&quot;.

Puede lanzar la excepción ScribusError si el documento no se puede abrir.
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

Guarda el documento actual con su nombre actual, y devuelve true si se 
guarda correctamente. Si el documento no se había guardado todavía, 
muestra un diálogo de guardar archivo. 

Si se produce un error al guardar, actualmente no hay forma de informarlo.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;)

Guarda el documento actual con el nombre &quot;name&quot; (que puede ser un ruta 
absoluta o relativa).

Puede lanzar la excepción ScribusError si se produce un error al guardar.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Establece la información básica del documento: autor, información y 
descripción. Todos los parámetros son strings.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Establece los márgenes del documento, Izquierda(lr), Derecha(rr), Arriba(tr) 
y Abajo(br). Los márgenes se expresan en las unidades del documento - ver 
las constantes UNIT_&lt;tipo&gt;.
</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Cambia las unidades de medida del documento. Los valores posibles para &quot;unit&quot; 
son las constantes UNIT_&lt;tipo&gt;.

Puede lanzar una excepción ValueError si se pasa una unidad no válida.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (constante de unidad de Scribus)

Devuelve las unidades de medida del documento. El valor devuelto será una 
de las constantes UNIT_*:
UNIT_INCHES (pulgadas), UNIT_MILLIMETERS (milimetros), 
UNIT_PICAS (picas), UNIT_POINTS (puntos).
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;)

Carga en el documento actual los estilos de párrafo del documento 
de Scribus que se encuentra en &quot;filename&quot;.
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

Establece el tipo de documento. Para que las páginas se muestren enfrentadas 
el primer parámetro debe ser FACINGPAGES, para desactivar las páginas 
enfrentadas debe ser NOFACINGPAGES. Si se quiere que la primera página esté 
a la izquierda el segundo parámetro debe ser FIRSTPAGELEFT, para que esté a 
la derecha debe ser FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string

Devuelve el nombre del color de línea del objeto &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer

Devuelve el ancho de línea del objeto &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer

Devuelve la intesnsidad del color de línea del objeto &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Devuelve el estilo de borde de línea del objeto &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
Los tipos de unión son: 
JOIN_BEVEL (biselada), JOIN_MITTER (de inglete), JOIN_ROUND (redondeada)
</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (ver constantes)

Devuelve el estilo de extremo de línea del objeto &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
Los tipos de extremo son: 
CAP_FLAT (plano), CAP_ROUND (redondeado), CAP_SQUARE (cuadrado)
</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (ver constantes)

Devuelve el estilo de línea del objeto &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado. 
Las constantes de estilos de línea son:
LINE_DASH (a trazos), LINE_DASHDOT (raya punto), 
LINE_DASHDOTDOT (raya punto punto), LINE_DOT (a puntos), 
LINE_SOLID (sólida)
</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Devuelve el valor de saturación del color de relleno del objeto &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">getCornerRadius([&quot;name&quot;]) -&gt; integer

Devuelve el radio de las esquinas del objeto &quot;name&quot;. 
El radio se expresa en puntos.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Devuelve un par (x, y) que contiene los valores de escalado del 
marco de imagen &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string

Devuelve el nombre de archivo de la imagen en el marco de imagen &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="obsolete">getPosition([&quot;name&quot;]) -&gt; (x,y)

Devuelve un par (x, y) con la posición del objeto &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
Las posiciones están expresadas en las unidades actuales del documento 
- ver UNIT_&lt;tipo&gt; para más referencias.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (width,height)

Devuelve un par (ancho, alto) con el tamaño del objeto &quot;name&quot;.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
El tamaño está expresado en las unidades actuales del documento 
- ver UNIT_&lt;tipo&gt; para más referencias.
</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;name&quot;]) -&gt; integer

Devuelve la rotación del objeto &quot;name&quot;. El valor está expresado 
en grados y si es positivo es en el sentido de las agujas del reloj.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

Devuelve una lista que contiene los nombres de todos los objetos 
de la página actual.
</translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;name&quot;])

Mueve el objeto &quot;name&quot; a una nueva posición. Las coordenadas están 
expresadas en las unidades actuales del documento (ver constantes UNIT).
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
Si el objeto a mover pertenece a un grupo, se mueve todo el grupo.
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
        <translation>rotateObject(rot [, &quot;name&quot;])

Rota el objeto &quot;name&quot; &quot;rot&quot; grados de forma relativa. El objeto se rota sobre 
el objeto que está seleccionado como punto de rotación - por defecto es 
el vértice superior izquierdo con cero grados de rotación. Los valores positivos 
significan rotación en sentido contrario a las agujas del reloj cuando se usa el 
punto de rotación por defecto.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">rotateObjectAbs(rot [, &quot;name&quot;])

Establece la rotación del objeto &quot;name&quot; a &quot;rot&quot;. Los valores positivos 
significan rotación en sentido contrario a las agujas del reloj.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;])

Redimensiona el objeto &quot;name&quot; a la anchura (width) y altura (height) dados.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

Devuelve el nombre del objeto seleccionado. &quot;nr&quot;, si se especifica, indica 
el número del objeto seleccionado, p. ej. 0 significa el primer objeto 
seleccionado, 1 el segundo objeto seleccionado, etc.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer

Devuelve el número de objetos seleccionados.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;)

Selecciona el objeto con el nombre &quot;name&quot;.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Deselecciona todos los objetos del documento entero.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list)

Agrupa los objetos nombrados en &quot;list&quot;. &quot;list&quot; debe contener los nombres 
de los objetos a ser agrupados. 
Si no se pasa una lista se usan los objetos que estén seleccionados.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;)

Destruye el grupo al que pertenece el objeto &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
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
        <translation>scaleGroup(factor [,&quot;name&quot;])

Escala el grupo al que pertenece el objeto &quot;name&quot;. Los valores mayor de 1 
agrandan el grupo, los menores de 1 lo hacen más pequeño, p. ej. un valor 
de 0.5 escala el grupo al 50% de su tamaño original, un valor de 1.5 escala 
el grupo a 150% de su tamaño original. 
El valor de &quot;factor&quot; debe ser mayor de cero.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.

Puede lanzar la excepción ValueError si se pasa un factor de escala no válido. 
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Carga la imagen &quot;filename&quot; en el marco de imagen &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.

Puede lanzar la excepción WrongFrameTypeError si el marco de destino 
no es un marco de imagen.
</translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;name&quot;])

Establece los factores de escalado de la imagen en el marco de 
imagen &quot;name&quot;. Un valor de 1 significa 100%.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.

Puede lanzar la excepción WrongFrameTypeError si el marco de destino 
no es un marco de imagen.
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

Bloquea el objeto &quot;name&quot; si está desbloqueado o lo desbloquea en 
caso contrario. Devuelve true si se bloquea. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

Devuelve true si el objeto &quot;name&quot; está bloqueado.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Devuelve una lista con los nombres de todas las fuentes disponibles.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list of tuples

Da una mayor información sobre las fuentes. Devuelve una lista de vectores con: 
[ (nombre de Scribus, Familia, Nombre real, empotrar subconjunto (1|0), 
empotrar PS (1|0), archivo de fuente), (...), ...]
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Devuelve una lista con los nombres de todas las capas definidas.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;)

Establece la capa activa a la capa con nombre &quot;name&quot;.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si la el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Devuelve el nombre de la capa activa actual.
</translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Envía el objeto &quot;name&quot; a la capa &quot;layer&quot;. La capa debe existir.
Si no se pasa un nombre, se usa el objeto que esté seleccionado.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;layer&quot;, visible)

Establece si la capa &quot;layer&quot; es visible o no. 
Si &quot;visible&quot; es false la capa será invisible.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;layer&quot;, printable)

Establece si la capa &quot;layer&quot; es imprimible o no. 
Si &quot;printable&quot; es false la capa no se imprimirá.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;layer&quot;)

Borra la capa con el nombre &quot;layer&quot;. No ocurre nada si la capa no existe o 
si es la única capa del documento.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer)

Crea una nueva capa co el nombre &quot;name&quot;.

Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string

Devuelve una cadena con el código del idioma usado.
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
        <translation>createEllipse(x, y, width, height, [&quot;name&quot;]) -&gt; string

Crea una elipse en la página actual y devuelve su nombre. 
Las coordenadas x, y, anchura y altura se expresan en las unidades 
actuales del documento (ver constantes UNIT). &quot;name&quot; debería ser 
un identificador único para el objeto porque se referenciará con ese 
nombre después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando.
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
        <translation>createImage(x, y, width, height, [&quot;name&quot;]) -&gt; string

Crea un marco de imagen en la página actual y devuelve su nombre. 
Las coordenadas x, y, anchura y altura se expresan en las unidades 
actuales del documento. &quot;name&quot; debería ser un identificador único 
para el objeto porque se referenciará con ese nombre después. 
Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando.
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
        <translation>createText(x, y, width, height, [&quot;name&quot;]) -&gt; string

Crea un marco de texto en la página actual y devuelve su nombre. 
Las coordenadas x, y, anchura y altura se expresan en las unidades 
actuales del documento (ver constantes UNIT). &quot;name&quot; debería ser 
un identificador único para el objeto porque se referenciará con ese 
nombre después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando.
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
        <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; string

Crea una línea del punto (x1, y1) al punto (x2, y2) y devuelve su 
nombre. Las coordenadas se expresan en las unidades actuales 
del documento (ver constantes UNIT). &quot;name&quot; debería ser un 
identificador único para el objeto porque se referenciará con ese 
nombre después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando.
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
        <translation>createPolyLine(list, [&quot;name&quot;]) -&gt; string

Crea una polilínea y devuelve su nombre. Los puntos de la polilínea 
se almacenan en la lista &quot;list&quot; en el orden siguiente: [x1, y1, x2, y2, ...xn, yn]. 
Las coordenadas se expresan en las unidades actuales del documento 
(ver constantes UNIT). &quot;name&quot; debería ser un identificador único para el 
objeto porque se referenciará con ese nombre después. Si no se le da 
un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando. 
Puede lanzar la excepción ValueError si se pasa un número de puntos 
insuficiente, o si el número de valores es impar.
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
        <translation>createPolygon(list, [&quot;name&quot;]) -&gt; string

Crea un polígono y devuelve su nombre. Los puntos del polígono se 
almacenan en la lista &quot;list&quot; en el orden siguiente: [x1, y1, x2, y2, ...xn, yn]. 
Al menos se necesitan 3 puntos. No se necesita repetir el primer punto 
para cerrar el polígono, se cierra automáticamente conectando el primer 
punto con el último. Las coordenadas se expresan en las unidades actuales 
del documento (ver constantes UNIT). &quot;name&quot; debería ser un identificador 
único para el objeto porque se referenciará con ese nombre después. 
Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando. 
Puede lanzar la excepción ValueError si se pasa un número de puntos 
insuficiente, o si el número de valores es impar.
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
        <translation>createBezierLine(list, [&quot;name&quot;]) -&gt; string

Crea una curva de bezier y devuelve su nombre. Los puntos de la 
curva de bezier se almacenan en la lista &quot;list&quot; en el orden siguiente: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn, yn, kxn, kyn]. 
En la lista de puntos, x e y son las coordenadas del punto y kx y ky 
el punto de control de la curva. Las coordenadas se expresan en las 
unidades actuales del documento (ver constantes UNIT). &quot;name&quot; debería 
ser un identificador único para el objeto porque se referenciará con ese 
nombre después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando. 
Puede lanzar la excepción ValueError si se pasa un número de puntos 
insuficiente, o si el número de valores es impar.
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
        <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -&gt; string

Crea un texto adjunto a un trazado, adjuntando el objeto&quot;textbox&quot; al objeto 
&quot;beziercurve&quot; y devuelve su nombre. Las coordenadas se expresan en las 
unidades actuales del documento (ver constantes UNIT). &quot;name&quot; debería 
ser un identificador único para el objeto porque se referenciará con ese 
nombre después. Si no se le da un nombre, Scribus creará uno. 

Puede lanzar la excepción NameExistsError si se pasa un nombre que 
ya se está usando. 
Puede lanzar la excepción NotFoundError si alguno de los objetos no existe.
</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;])

Borra el objeto con el nombre &quot;name&quot;. 
Si no se pasa un nombre, se usa el objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Activa o desactiva la característica &quot;el texto fluye alrededor del marco&quot; 
del objeto &quot;name&quot;, según el valor del parámetro booleano &quot;state&quot;. 
Si no se pasa &quot;state&quot;, se invierte dicha característica.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; bool

Comprueba si existe en el documento un objeto con el nombre &quot;name&quot;. 
Si no se pasa un nombre, devuelve true si hay algún objeto seleccionado.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Aplica el estilo &quot;style&quot; al objeto &quot;name&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list

Devuelve una lista con los nombres de todos los estilos 
de párrafo del documento.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer

Devuelve el número de la página actual. Los números de página van del 1 en 
adelante, independientemente del número mostrado en la primera página.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Redibuja todas las páginas.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;name&quot;)

Guarda la página actual como un EPS al archivo &quot;name&quot;.

Puede lanzar la excpeción ScribusError si hubo un error al guardar.
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

Borra la página &quot;nr&quot;. No hace nada si el documento sólo tiene una página. 
Los números de página van del 1 en adelante, independientemente del 
mostrado en la primera página. 

Puede lanzar la excepción IndexError si el número de página está fuera de rango.
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

Cambia a la página &quot;nr&quot; (es decir, hace la página actual la página &quot;nr&quot;). 
Nota que actualmente no cambia la página que se está mostrando, sólo 
establece la página sobre la que operan los comandos del script.

Puede lanzar la excepción IndexError si el número de página está fuera de rango.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; integer

Devuelve el número de páginas del documento.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Devuelve una lista que contiene las posiciones de las guías horizontales. 
Los valores están en las unidades actuales del documento - ver las 
constantes UNIT_&lt;tipo&gt;.
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

Establece las guías horizontales. El parámetro debe ser una lista de 
posiciones medidas en las unidades actuales del documento - ver 
las constantes UNIT_&lt;tipo&gt;.

Ejemplo: setHGuides(getHGuides() + [200.0, 210.0] # añade guías sin perder ninguna
         setHGuides([90,250]) # reemplaza las guías actuales
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Ver getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Ver setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple

Devuelve un vector con las dimensiones de página medidas en las unidades 
actuales del documento. Ver las constantes UNIT_&lt;tipo&gt; y getPageMargins()
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

Devuelve una lista de vectores con los objetos de la página actual. 
El vector es (nombre, tipoDeObjeto, orden). 
P. ej. [(&apos;Texto1&apos;, 4, 0), (&apos;Imagen1&apos;, 2, 1)] significa que el objeto &apos;Texto1&apos; 
es un marco de texto (tipo 4) y es el primero de la página...
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Establece el color de relleno del objeto &quot;name&quot; al color &quot;color&quot;. 
&quot;color&quot; es el nombre de uno de los colores definidos. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Establece el color de línea del objeto &quot;name&quot; al color &quot;color&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(width, [&quot;name&quot;])

Establece el ancho de línea del objeto &quot;name&quot; a &quot;width&quot;. &quot;width&quot; debe estar 
en el rango de 0.0 a 12.0 inclusive, y se miede en puntos.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si el ancho de línea está fuera de rango.
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
        <translation>setLineShade(shade, [&quot;name&quot;])

Establece la saturación del color de línea del objeto &quot;name&quot; a &quot;shade&quot;.
&quot;shade&quot; debe ser un valor entero en el rango de 0 (más claro) a 100
(color de mayor intensidad). 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si la saturación está fuera de rango.
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Establece el estilo de unión de línea del objeto &quot;name&quot; al estilo &quot;join&quot;.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
Hay constantes predefinidas para &quot;join&quot; - JOIN_&lt;tipo&gt;.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;name&quot;])

Establece el estilo de extremo de línea del objeto &quot;name&quot; al estilo &quot;cap&quot;.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
Hay constantes predefinidas para &quot;cap&quot; - CAP_&lt;tipo&gt;.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;])

Establece el estilo de línea del objeto &quot;name&quot; al estilo &quot;style&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
Hay constantes predefinidas para &quot;style&quot; - LINE_&lt;tipo&gt;.
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
        <translation>setFillShade(shade, [&quot;name&quot;])

Establece la saturación del color de relleno del objeto &quot;name&quot; a &quot;shade&quot;.
&quot;shade&quot; debe ser un valor entero en el rango de 0 (más claro) a 100
(color de mayor intensidad). 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si la saturación está fuera de rango.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;name&quot;])

Establece el radio de las esquinas del objeto &quot;name&quot;. El radio está 
expreado en puntos. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si el radio está fuera de rango.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Establece el estilo de línea del objeto &quot;name&quot; al estilo de nombre &quot;namedStyle&quot;.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción NotFoundError si el estilo de línea no existe.
</translation>
    </message>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;name&quot;]) -&gt; string

Devuelve el nombre de la fuente del marco de texto &quot;name&quot;. Si el 
marco de texto tiene texto seleccionado, devuelve el del primer 
caracter de la selección. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

Devuelve la longitud del texto en el marco de texto &quot;name&quot;.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;name&quot;]) -&gt; string

Devuelve el texto del marco de texto &quot;name&quot;.  Si el marco de texto 
tiene texto seleccionado, devuelve el texto seleccionado. Devuelve 
todo el texto del marco, no solo el que se ve actualmente.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getAllText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot; and of all text frames which are
linked with this frame. If this textframe has some text selected, the selected
text is returned. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>getAllText([&quot;name&quot;]) -&gt; string

Devuelve el texto del marco de texto &quot;name&quot; y de todos los marcos de texto 
que están enlazados con este marco. Si el marco de texto tiene texto 
seleccionado, devuelve el texto seleccionado.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Devuelve el interlineado del marco de texto &quot;name&quot; expresado en puntos. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Devuelve la distancia entre columnas del marco de texto &quot;name&quot; expresado 
en puntos. Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

Devuelve el número de columnas del marco de texto &quot;name&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Establece el texto del marco de texto &quot;name&quot; al texto de la cadena &quot;text&quot;. 
El texto debe estar codificado en UTF8 - usa p. ej. unicode(&quot;text&quot;, &apos;iso-8859-2&apos;). 
Ver las FAQ para más detalles. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setFont(&quot;font&quot;, [&quot;name&quot;])

Sets the font of the text frame &quot;name&quot; to &quot;font&quot;. If there is some text
selected only the selected text is changed.  If &quot;name&quot; is not given the
currently selected item is used.

May throw ValueError if the font cannot be found.
</source>
        <translation>setFont(&quot;font&quot;, [&quot;name&quot;])

Establece la fuente del marco de texto &quot;name&quot; a &quot;font&quot;. Si hay texto 
seleccionado sólo cambia la del texto seleccionado.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si no se puede encontrar la fuente.
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
        <translation>setFontSize(size, [&quot;name&quot;])

Establece el tamaño de fuente del marco de texto &quot;name&quot; a &quot;size&quot;. 
&quot;size&quot; debe ser un valor en puntos. Si hay texto seleccionado sólo 
cambia la del texto seleccionado. &quot;size&quot; debe ser un entero en el 
rango 1 a 512. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si el tamaño está fuera de rango.
</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(size, [&quot;name&quot;])

Establece el interlineado del marco de texto &quot;name&quot; a &quot;size&quot;. 
&quot;size&quot; es un valor en puntos. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado. 

Puede lanzar la excepción ValueError si el interlineado está fuera de rango.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(size, [&quot;name&quot;])

Establece la distancia entre columnas del marco de texto &quot;name&quot; al 
valor &quot;size&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si la distancia entre columnas 
está fuera de rango (debe ser positiva).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;name&quot;])

Establece el número de columnas del marco de texto &quot;name&quot; al entero &quot;nr&quot;.
Si no se pasa un nombre, se aplica al objeto que esté seleccionado. 

Puede lanzar la excepción ValueError si el número de columnas es menor de uno.
</translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(align, [&quot;name&quot;])

Establece el alineamiento del marco de texto &quot;name&quot; al alineamiento 
indicado. &quot;align&quot; debería ser una de las constantes ALIGN_ definidas 
en este módulo - ver dir(scribus).
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.

Puede lanzar la excepción ValueError si la constante de la alineamiento 
no es válida.
</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;name&quot;])

Borrar el texto del marco de texto &quot;name&quot;. Si hay texto seleccionado sólo 
se borra el texto seleccionado. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Establece el color del texto marco de texto &quot;name&quot; al color &quot;color&quot;. Si 
hay texto seleccionado sólo se cambia el texto seleccionado. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Establece el color del trazo del texto a &quot;color&quot;. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
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
        <translation>setTextShade(shade, [&quot;name&quot;])

Establece la saturación del color del texto del objeto &quot;name&quot; a &quot;shade&quot;. 
Si hay texto seleccionado, sólo cambia el texto seleccionado. &quot;shade&quot; 
debe ser un valor entero en el rango de 0 (más claro) a 100 (color de 
mayor intensidad).
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Enlaza dos marcos de texto. El marco llamado &quot;fromname&quot; se enlaza al marco 
llamado &quot;toname&quot;. El marco de destino debe ser un marco de texto vacío y 
no debe estar enlazado a otros ni desde otros marcos de texto.

Puede lanzar la excepción ScribusException si se violan las reglas de enlazado.
</translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;name&quot;)

Elimina el objeto &quot;name&quot; del flujo de enlazado de marcos de texto. Si el 
marco estaba en medio de una cadena, los marcos anterior y siguiente 
se enlazarán, p. ej. &apos;a-&gt;b-&gt;c&apos; pasa a ser &apos;a-&gt;c&apos; al ejecutar unlinkTextFrames(b) 

Puede lanzar la excepción ScribusException si se violan las reglas de enlazado.
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;])

Convierte el marco de texto &quot;name&quot; a contornos. 
Si no se pasa un nombre, se aplica al objeto que esté seleccionado.
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Borra la barra de progreso de Scribus. Se llama antes de usar 
la barra de progreso. Ver progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Establece el máximo número de pasos de la barra de progreso al 
número especificado. Ver progressSet.
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

Establece la posición de la barra de progreso a &quot;nr&quot;, un valor relativo a 
lo puesto anteriormente con progressTotal. La barra de progreso usa el 
concepto de pasos, se le dice el número de pasos totales y el número 
pasos completado y mostrará el porcentaje de pasos que se han 
completado. Se puede especificar el número de pasos con progressTotal(). 
El número actual de pasos se establece con progressSet(). Se rebobina al 
principio con progressReset(). 
[basado en la información tomada de la documentación de Qt de Trolltech]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[¡NO SOPORTADO!] 
Esto podría romper cosas, así que no se utiliza de momento.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool)

Activa o desactiva el icono de guardar en la barra de iconos de Scribs y la 
opción de menú Guardar. Es útil llamarlo cuando estás cambiando el documento, 
porque Scribus no notará automáticamente cuando se cambie el documento 
con un script.
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

Establece la opción escalar al tamaño del marco a &quot;scaletoframe&quot; en el marco de
imagen seleccionado o especificado por &quot;name&quot;. 
Si se especifica &quot;proportional&quot;, establece la opción mantener la relación de aspecto
a &quot;proportional&quot;. 
Tanto &quot;scaletoframe&quot; como &quot;proportional&quot; son booleanos.

Puede lanzar la excepción WrongFrameTypeError.
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
        <translation type="obsolete">selectText(start, count, [&quot;name&quot;])

Selecciona &quot;count&quot; caracteres del marco de texto &quot;name&quot; empezando 
desde el carácter &quot;start&quot;. Los caracteres se cuentan desde 0. Si &quot;count&quot; 
es cero, se perderá cualquier selección que hubiera. Si &quot;count&quot; es -1, 
se seleccionará todo el texto del marco. Si no se pasa &quot;name&quot; se usará 
el objeto seleccionado actualmente.

Puede lanzar la excepción IndexError si la selección está fuera de los 
límites del texto.
</translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation type="obsolete">register_macro_code(name, sourcetext, accel=&apos;&apos;)

Crea una macro llamada &quot;name&quot; evaluando el código fuente &quot;sourcetext&quot;.
&quot;sourcetext&quot; debe seguir las mismas reglas que las macros creadas en 
la interfaz gráfica. Si se proporciona &quot;accel&quot; se usará para establecer un 
acceso rápido para la macro.
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
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Devuelve si la capa &quot;layer&quot; es imprimible o no, un valor de True significa 
que la capa &quot;layer&quot; se puede imprimir, un valor de False significa que se 
ha desactivado la impresión de la capa&quot;layer&quot;.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
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
        <translation type="obsolete">getColorAsRGB(&quot;name&quot;) -&gt; tuple

Devuelve un vector (R,G,B) conteniendo las tres componentes de color 
del color &quot;name&quot; del actual documento, convertido al espacio de color 
RGB. Si no hay ningún documento abierto, devuelve el valor de dicho 
color de los colores por defecto.

Puede lanzar la excepción NotFoundError si no se puede encontrar dicho color. 
Puede lanzar la excepción ValueError si el nombre del color no es válido.
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
        <translation>renderFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size, format=&quot;PPM&quot;) -&gt; bool

Crea una previsualización de la fuente &quot;name&quot; con el texto &quot;sample&quot; y 
de tamaño &quot;size&quot;. Si &quot;filename&quot; no es &quot;&quot;, la imagen se guarda en&quot;filename&quot;. 
De otra forma la imagen se devuelve como una cadena. El argumento 
opcional &quot;format&quot; especifica el formato de imagen a generar, y admite 
cualquier formato permitido por QPixmap.save(). Los formatos comunes 
son PPM, JPEG, PNG y XPM.

Puede lanzar la excepción NotFoundError si no se puede encontrar la 
fuente indicada.
Puede lanzar la excepción ValueError si &quot;filename&quot; o &quot;sample&quot; son 
cadenas vacías.
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
        <translation type="obsolete">isLayerVisible(&quot;layer&quot;) -&gt; bool

Devuelve si la capa &quot;layer&quot; es visible o no, un valor de True significa 
que la capa &quot;layer&quot; es visible, un valor de False significa que la capa
&quot;layer&quot; es invisible.

Puede lanzar la excepción NotFoundError si no se puede encontrar la capa. 
Puede lanzar la excepción ValueError si el nombre de la capa no es válido.
</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Establece si (toggle = 1) el marco de texto &quot;name&quot; es un marcador o no. 
Si no se pasa &quot;name&quot; se usa el objeto actualmente seleccionado.

Puede lanzar la excepción WrongFrameTypeError si el marco de destino 
no es un marco de texto.
</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Devuelve si el marco de texto &quot;name&quot; es un marcador de PDF. 
Si no se pasa &quot;name&quot; se usa el objeto actualmente seleccionado.

Puede lanzar la excepción WrongFrameTypeError si el marco de destino 
no es un marco de texto.
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Devuelve los márgenes de página como un vector (arriba, izquierda, derecha, abajo) 
en las unidades actuales. Ver las constantes UNIT_&lt;tipo&gt; y getPageSize().
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
        <translation type="obsolete">insertText(&quot;text&quot;, pos, [&quot;name&quot;])

Inserta el texto &quot;text&quot; en la posición &quot;pos&quot; en el marco de texto &quot;name&quot;. 
El texto debe estar codificado en UTF (ver setText() como referencia). 
El primer carácter tiene índice 0. Insertar en la posición -1 añade &quot;text&quot; 
al marco. Si no se pasa &quot;name&quot; se usará el objeto actualmente seleccionado.

Puede lanzar la excepción IndexError para una inserción fuera de rango.
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>masterPageNames()

Returns a list of the names of all master pages in the document.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>editMasterPage(pageName)

Enables master page editing and opens the named master page
for editing. Finish editing with closeMasterPage().
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>createMasterPage(pageName)

Creates a new master page named pageName and opens it for
editing.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>deleteMasterPage(pageName)

Delete the named master page.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>zoomDocument(double)

Zoom the document in main GUI window. Actions have whole number
values like 20.0, 100.0, etc. Zoom to Fit uses -100 as a marker.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus %1</source>
        <translation>Acerca de Scribus %1</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Versión de Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation type="obsolete">Build-ID:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Acerca de</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Equipo de Desarrollo:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contribuciones de:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Versión de Windows:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Documentación Oficial:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Otra Documentación:</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utores</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Traducciones Oficiales y Traductores:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Catalán:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Checo:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Danés:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Inglés (Británico):</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Esperanto:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Finlandés:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Francés:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Alemán:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Italiano:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Coreano:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Noruego:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Polaco:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Ruso:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Serbio:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Eslovaco:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Esloveno:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Español:</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Sueco:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Ucraniano:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Galés:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Anteriores Colaboradores en la Traducción:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Vasco:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Brasileño:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Búlgaro:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Gallego:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Hungaro:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Lituano:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Turco:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Traducciones</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Página inicial</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referencia en línea</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Errores y Peticiones de Características</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Lista de Correo</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;En línea</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Este panel muestra la versión, fecha de construcción y el 
soporte de librerías con que se ha compilado Scribus. 
C-C-T significa soporte de C=CUPS C=littlecms T=TIFF.
Las bibliotecas que falten se indican con un * 
</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation type="obsolete">%1 %2 %3 </translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation type="obsolete">Afrikaans:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation type="obsolete">Portugués (Brasileño):</translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%1 %2 %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>%3-%2-%1 %4 %5</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Using Ghostscript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Ghostscript version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Scribus Version %1&lt;/b&gt;&lt;p&gt;%2&lt;br/&gt;%3 %4&lt;br/&gt;%5&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Build ID:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>February</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mac OS&amp;#174; X Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Windows&amp;#174; Port:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>Filename:</source>
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
        <source>Description:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ActionManager</name>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Abrir...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">Revertir al guardado</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">Rec&amp;olectar para salida...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Cargar texto...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">&amp;Añadir texto...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Guardar el &amp;texto...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Guardar la página como &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">Guardar como P&amp;DF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">Co&amp;nfiguración del documento...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Imprimir...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">&amp;Salir</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">Re&amp;hacer</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished">Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">B&amp;orrar contenidos</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">Seleccionar &amp;todo</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Buscar/Reemplazar...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">C&amp;olores...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">Estilos de &amp;párrafo...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">Estilos de &amp;línea...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">&amp;Preferencias...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Otro...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">&amp;Izquierda</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">&amp;Centro</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">&amp;Derecha</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">&amp;Justificado</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished">&amp;Forzado</translation>
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
        <translation type="unfinished">&amp;Subrayado</translation>
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
        <translation type="unfinished">&amp;Esquema</translation>
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
        <translation type="unfinished">Ta&amp;buladores...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">Duplicación &amp;múltiple</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">Agr&amp;upar</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished">&amp;Desagrupar</translation>
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
        <translation type="unfinished">&amp;Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">&amp;Subir</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">Enviar al Álbu&amp;m de Recortes</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">&amp;Imagen visible</translation>
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
        <translation type="unfinished">&amp;Marcador PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">&amp;Anotación PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">&amp;Propiedades de las Anotaciones</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">&amp;Propiedades de Campo</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">&amp;Editar forma...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">&amp;Adjuntar Texto al Trazado</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">&amp;Desadjuntar Texto del Trazado</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">&amp;Combinar Polígonos</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">Dividir &amp;Polígonos</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">Curva de &amp;Bézier</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished">&amp;Contorno</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">Marco de &amp;texto</translation>
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
        <translation type="unfinished">&amp;Insertar...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">&amp;Borrar...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Mover...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">Administrar &amp;Guías...</translation>
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
        <translation type="unfinished">&amp;Miniaturas</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Mostrar Má&amp;rgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Mostrar &amp;Marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">Mostrar &amp;Imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Mostrar &amp;Cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Mostrar &amp;Guías</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">Mostrar Líneas de &amp;Base</translation>
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
        <translation type="unfinished">&amp;Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished">A&amp;justar a las Guías</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished">&amp;Propiedades</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">Álb&amp;um de recortes</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">&amp;Capas</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Marcadores</translation>
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
        <translation type="unfinished">&amp;Herramientas</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">Herramientas de P&amp;DF</translation>
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
        <translation type="unfinished">&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">&amp;Linea</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Rotar objeto</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">Acercar o alejar</translation>
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
        <translation type="unfinished">Editar contenidos del marco</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">&amp;Editar texto...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Enlazar marcos de texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Desenlazar marcos de texto</translation>
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
        <translation type="unfinished">Editar el texto con el editor interno</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Insertar marco de texto</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Insertar tabla</translation>
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
        <translation type="unfinished">Insertar linea a mano alzada</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">Ad&amp;ministrar imágenes</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">&amp;Dividir en sílabas</translation>
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
        <translation type="unfinished">&amp;Acerca de Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">Acerca de &amp;Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">Ayuda &amp;emergente</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">&amp;Manual de Scribus</translation>
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
        <translation type="unfinished">Pre&amp;visualizar</translation>
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
        <translation type="unfinished">&amp;Cascada</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished">&amp;Mosaico</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>More Info...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paste Contents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Printing Enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Flip Horizontally</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Flip Vertically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show Rulers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <comment>Document Outline Palette</comment>
        <translation type="unfinished">&amp;Esquema</translation>
    </message>
    <message>
        <source>Solidus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Dot</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>En Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Em Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thin Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Thick Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mid Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hair Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Smart Hyphen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Non Breaking Dash</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Non Breaking Space</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="unfinished">Insertar Número de Página</translation>
    </message>
    <message>
        <source>ff</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>fl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffi</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ffl</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>ft</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>st</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Opciones Avanzadas</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Voltear página(s) &amp;horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Voltear página(s) &amp;verticalmente</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Aplicar &amp;eliminación del color subyacente (UCR)</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Aplicar perfiles &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">PostScript Nivel &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">PostScript Nivel &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">PostScript Nivel &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Crea PostScript Nivel 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Crea PostScript Nivel 2 solamente, ten en
cuenta que puede crear archivos enormes.</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Crea PostScript Nivel 1 solamente, ten en
cuenta que puede crear archivos enormes.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="obsolete">Establecer Tamaño del Medio</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="obsolete">Te permite establecer explícitamente, 
el tamaño del medio del archivo postscript. 
No recomendado a menos que 
lo pida tu impresora.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Distribuir/Alinear</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Alinear</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horizontal</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Lados izquierdos</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Centro</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Lados derechos</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Entre:</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;No cambiar</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">A&amp;linear</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">&amp;Desplazamiento</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Distribuir e&amp;quitativamente</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertical</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Lados superiores</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Lados inferiores</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">En&amp;tre:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">N&amp;o cambiar</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">Al&amp;inear</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">De&amp;splazamiento</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Distribuir eq&amp;uitativamente</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">A&amp;plicar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
        <translation type="unfinished">Alinear</translation>
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
        <translation type="unfinished">Página</translation>
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
        <translation type="obsolete">Atención</translation>
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
        <translation>Alinear Texto a la Izquierda</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinear Texto a la Derecha</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Centrar Texto</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Justificar Texto</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Justificación forzada del Texto</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Propiedades de campo</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Tipo:</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de selección</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Cuadro combinado</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Consejo:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Fuente a usar para PDF-1.3:</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Borde</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Ninguno</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Anchura:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Delgado</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Ancho</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Estilo:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>A trazos</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Biselado</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Recuadro</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Otro</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Sólo Lectura</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Requerido</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>No exportar valor</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilidad:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visible</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Oculto</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>No Imprimir</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>No Mostrar</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Apariencia</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Texto al Presionar el Botón</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Texto al Pasar por Encima</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Iconos</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Usar Iconos</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pulsado</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Cursor encima</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Disposición del icono...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Realzar</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Invertir</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Contorneado</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Pulsar</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multilínea</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Contraseña</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Límite de</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>caracteres</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>No desplazar</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>No revisar ortografía</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Estilo de marca:</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>V</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Cruz</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamante</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Círculo</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Estrella</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Seleccionada por defecto</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editable</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Java Script</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ir a</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Enviar formulario</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Reiniciar formulario</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importar datos</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Se suelta el ratón</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Se presiona el ratón</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>El cursor entra</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>El cursor sale</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>Se obtiene el foco</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>Se pierde el foco</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editar...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Enviar a URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Enviar dato como HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importar datos desde:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>A archivo:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Cambiar...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Página:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Posición X:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Posición Y:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Campo formateado como:</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Sin formato</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Número</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Porcentaje</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Hora</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formato de Número</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimales:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Usar Símbolo de Moneda</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Símbolo de Moneda Delante</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formato de Porcentaje</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formato de Fecha</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formato de Hora</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Scripts Propios</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Pulsación:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>El valor no se valida</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>El valor debe ser mayor o igual que:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>y menor o igual que:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script propio de validación:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>El valor no se calcula</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Valor es el</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>total</translation>
    </message>
    <message>
        <source>product</source>
        <translation>producto</translation>
    </message>
    <message>
        <source>average</source>
        <translation>promedio</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>mínimo</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>máximo</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>de los siguientes campos:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Elegir...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script de cálculo propio:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calcular</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Opción ignorada por PDF 1.3</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Introducir aquí una lista de campos separados por comas</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Se necesita como mínimo el icono Normal para usar iconos en los botones</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Imágenes (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Ejemplo:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Se cambia la selección</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <comment>highlight</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>None</source>
        <comment>action</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Propiedades de Anotación</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Enlace externo</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Enlace web externo</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Página:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Documentos PDF (*.pdf);; Todos los archivos (*)</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
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
        <translation type="unfinished">Alt+E</translation>
    </message>
    <message>
        <source>O&amp;dd pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">Todas las &amp;Páginas</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
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
        <source>to</source>
        <translation type="unfinished">a</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Apply the selected master page to even, odd or all pages within the following range</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Aplicar Plantilla</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Plantilla:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Aplicar a la página ac&amp;tual</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Aplicar a todas las páginas &amp;pares</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Aplicar a todas las páginas &amp;impares</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Aplicar &amp;desde la página:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">Hasta:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Ninguno</translation>
    </message>
</context>
<context>
    <name>Barcode</name>
    <message>
        <source>&amp;Barcode Generator...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus frontend for Pure Postscript Barcode Writer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGenerator</name>
    <message>
        <source>Error opening file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error creating preview</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>BarcodeGeneratorBase</name>
    <message>
        <source>Barcode Creator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation type="unfinished">&amp;Tipo:</translation>
    </message>
    <message>
        <source>C&amp;ode:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select one of the available barcode type here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The numeric representation of the code itself. See the help message below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the barcode samples</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Include text in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished">Alt+I</translation>
    </message>
    <message>
        <source>If checked, there will be numbers in the barcode too</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Guard whitespace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+G</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Draw arrows to be sure of space next the code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Colores</translation>
    </message>
    <message>
        <source>&amp;Background</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+B</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Background color - under the code lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Color of the lines in barcode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation type="unfinished">&amp;Texto</translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of the text and numbers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hints and help is shown here</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview of the result. 72dpi sample.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Álbum de recortes</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>C&amp;argar...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation type="obsolete">&amp;Pequeño</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation type="obsolete">&amp;Mediano</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation type="obsolete">&amp;Grande</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Previsualizar</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation type="obsolete">Álbum de recortes (*.scs);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objeto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nueva entrada</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; is not unique.
Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a Scrapbook Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Elige un directorio</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Mover Marcador</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Insertar Marcador</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Marcadores</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Disposición del icono</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Disposición:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Sólo texto</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Sólo icono</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Texto debajo del icono</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Texto encima del icono</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Texto a la derecha del icono</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Texto a la izquierda del icono</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Texto superpuesto al icono</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Escalado:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Siempre</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Cuando el icono es demasiado pequeño</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Cuando el icono es demasiado grande</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nunca</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Modo de escalado:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcional</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>No proporcional</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Icono</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Reiniciar</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Configuración de los Ajustes de Color</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Ac&amp;tivar Ajustes de Color</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Perfiles del Sistema</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">&amp;Imágenes:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Colores &amp;Sólidos:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Im&amp;presora:</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Modos de Visualización</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptivo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Imp&amp;resora:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sim&amp;ular impresora en la pantalla</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Indicar colores fuera de &amp;gama</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Usar compensación de punto &amp;negro</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Perfil de color por defecto para imágenes importadas</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Perfil de color por defecto para colores sólidos en la página</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color que hayas generado o que te haya dado el fabricante.
Debería ser específico para tu monitor y no uno genérico (como sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>El perfil de color del fabricante para tu modelo de impresora.
Debería ser específico para tu impresora y no uno genérico (como sRGB).</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Activar comprobación software (soft proofing) de cómo se imprimirán 
los colores del documento, basada en el perfil de impresora seleccionado.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Una forma de mostrar en la pantalla los colores que no se imprimirán apropiadamente.
Esto requiere perfiles muy precisos y sólo sirve como un aviso.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Un método de mejorar el contraste en las fotos. Se recomienda
que lo actives si tienes fotos en el documento</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Modo de visualización por defecto para tu monitor. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Modo de visualización por defecto para tu impresora. A menos que sepas
porqué cambiarlo, deberías elegir Colormetraje Relativo o Perceptivo.</translation>
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
        <translation>Editar color</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Modelo de color</translation>
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
        <translation>RGB Seguro para la Red</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Viejo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Mapa de color TSI (HSV)</translation>
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
        <translation>Barras de Color Dinámicas</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barras de Color Estáticas</translation>
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
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation type="obsolete">No puedes crear un color llamado &quot;%1&quot;.
Es un nombre reservado para el color transparente.</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation type="obsolete">El nombre del color ya existe</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It is a reserved name for transparent color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name of the color is not unique</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press and hold down the right mouse button. The Insert key inserts a Glyph into the Selection below and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation type="unfinished">Seleccionar Carácter:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Fuente:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Insertar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished">Inserta los caracteres en el texto en la posición del cursor</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Borra la selección actual</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Cierra este diálogo y vuelve a la edición de texto</translation>
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
        <translation type="unfinished">Griego</translation>
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
    <message>
        <source>&amp;Insert Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type in a four digit unicode value directly here</source>
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
        <translation type="unfinished">Documento</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Página </translation>
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
        <translation type="obsolete">Postscript</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Elegir Estilos</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Estilos Disponibles</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Elige un directorio</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Atención</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot collect the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ColorManager</name>
    <message>
        <source>Colors</source>
        <translation type="unfinished">Colores</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="unfinished">Borrar no &amp;usados</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="unfinished">Conjuntos de Colores</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="unfinished">Conjunto actual:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="unfinished">&amp;Guardar Conjunto de Colores</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="unfinished">Elige el conjunto de colores a cargar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="unfinished">Guarda el conjunto de colores actual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="unfinished">Eliminar del conjunto de colores del documento los que no se usen</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="unfinished">Crear un nuevo color en el actual conjunto</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="unfinished">Editar el color seleccionado</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="unfinished">Hacer una copia del color seleccionado</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="unfinished">Borrar el color seleccionado</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="unfinished">Establecer el conjunto actual como el conjunto de colores por defecto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Nombre:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="unfinished">Elige un nombre</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="unfinished">Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Copia de %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="unfinished">Nuevo color</translation>
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
        <translation type="unfinished">Color</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
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
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
        <translation type="unfinished">Co&amp;lor</translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
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
        <translation type="unfinished">Nuevo color</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olor Components...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Protanopia (Red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deuteranopia (Green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tritanopia (Blue)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color %1 exists already!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color %1 appended.</source>
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
        <translation type="unfinished">A&amp;plicar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Atención</translation>
    </message>
    <message>
        <source>None</source>
        <comment>color name</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>Custom</source>
        <comment>CommonStrings, custom page size</comment>
        <translation type="unfinished">Personalizado</translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Página izquierda</translation>
    </message>
    <message>
        <source>Middle</source>
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
        <source>Right Page</source>
        <translation type="unfinished">Página derecha</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Shade:</source>
        <translation>Saturación:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidad:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Degradado horizontal</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Degradado vertical</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Degradado diagonal</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Degradado diagonal cruzado</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Degradado radial</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Degradado lineal libre</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Degradado radial libre</translation>
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
        <source>Edit Line Color Properties</source>
        <translation>Editar color de línea</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editar color de relleno</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturación del color</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Método de relleno normal o gradiente</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Establece la transparencia del color seleccionado</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Color del objeto seleccionado</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
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
        <translation>Opciones de importación de CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Delimitador de campo:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Delimitador de valor:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>La primera fila es una cabecera</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <comment>delimiter</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Opciones de impresión</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Selección de páginas</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Todas</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Sólo pares</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Sólo impares</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Voltear</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Páginas por hoja</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Página por hoja</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Páginas por hoja</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opción</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Este panel muestra varias opciones de CUPS cuando se imprime.
Los parámetros exactos disponibles dependerán del driver de tu impresora.
Puedes confirmar el qué soporta CUPS seleccionando Ayuda &gt; Acerca de Scribus.
Busca los listados: C-C-T Esto significa soporte de C=CUPS C=littlecms T=TIFF
Las bibliotecas que falten se indican con un *</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. The exact parameters available will depend on your printer driver. You can confirm CUPS support by selecting Help &gt; About. Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support. Missing library support is indicated by a *</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Comprimir el archivo</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Incluir fuentes</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Ir al Directorio Documentos.
Se puede definir en las Preferencias.</translation>
    </message>
</context>
<context>
    <name>CwSetColor</name>
    <message>
        <source>Set Color Components</source>
        <translation type="unfinished"></translation>
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
        <source>HSV</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>H:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>V:</source>
        <translation type="unfinished"></translation>
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
        <source>C:</source>
        <translation type="unfinished">C:</translation>
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
        <source>Set &amp;RGB</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set C&amp;MYK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set &amp;HSV</source>
        <translation type="unfinished"></translation>
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
        <translation>Borrar color</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Color a borrar:</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Reemplazarlo con:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
        <translation>Borrar páginas</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Borrar desde:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>hasta:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DelStyle</name>
    <message>
        <source>Delete Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Replace With:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Sin estilo</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Fuente ausente</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">La fuente %1 no está instalada.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Usar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">en su lugar.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
</context>
<context>
    <name>DocIm</name>
    <message>
        <source>Importing failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Importing Word document failed 
%1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Información del documento</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Título:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>A&amp;utor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Palabras clave</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>De&amp;scripción:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Editor:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>Cola&amp;boradores:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Fecha:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Formato:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>&amp;Identificador:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Fuente:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relación:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Portada:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>De&amp;rechos:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">&amp;Documento</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>&amp;Información adicional</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">La persona u organización responsable principal de hacer el contenido del documento.
Este campo se puede empotrar en el documento de Scribus como referencia,
además de en los metadados de un PDF.</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Un nombre dado al documento.
Este campo se puede empotrar en el documento de Scribus como referencia,
además de en los metadados de un PDF.</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Un informe del contenido del documento.
Este campo es para una breve descripción o resumen del documento.
Se empotra en el PDF al exportar.</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">El tema del contenido del documento.
Este campo es para palabras clave del documento que deseas que se
empotren en un PDF, para ayudar a buscar e indexar archivos PDF.</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Una persona u organización responsable de hacer disponible el documento</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Una persona u organización responsable de hacer contribuciones al contenido del documento.</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Una fecha asociada con un evento en el ciclo de vida del documento, en formato AAAA-MM-DD, de acuerdo a ISO 8601.</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>La naturaleza o género del contenido del documento, p.ej. categorías, funciones, géneros, etc.</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">El formato físico o digital del documento. El tipo de medio y las dimensiones serían interesantes.
También son útiles los tipos MIME según RFC2045 y RFC2046.</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Una referencia ambigua al documento dentro de un contexto dado, como un ISBN o URI.</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Una referencia a un documento del que deriva el documento actual, p.ej. un ISBN o URI.</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">El idioma en que está escrito el contenido del documento, normalmente un código de idioma ISO-639
opcionalmente seguido con un guión y un código de país ISO-3166, p.ej. en-GB, fr-CH.</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Una referencia a un documento relacionado, posiblemente usando un identificador formal como un ISBN o URI.</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>La extensión o alcance del contenido del documento, posiblemente incluyendo localización, tiempo y rangos de jurisdicción.</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Información sobre los derechos mantenidos sobre el documento, p.ej. derechos de autor, patentes o marcas registradas.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
    <name>DocSections</name>
    <message>
        <source>Add a page numbering section to the document. The new section will be added after the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the currently selected section.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>1, 2, 3, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>i, ii, iii, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I, II, III, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>a, b, c, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A, B, C, ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;b&gt;Name:&lt;/b&gt; Optional name for section eg. Index&lt;br/&gt;&lt;b&gt;Shown:&lt;/b&gt; Select to show the page numbers in this section if there is one or more text frames setup to do so.&lt;br/&gt;&lt;b&gt;From:&lt;/b&gt; The page index for this section to start at.&lt;br/&gt;&lt;b&gt;To:&lt;/b&gt; The page index for this section to stop at.&lt;br/&gt;&lt;b&gt;Style:&lt;/b&gt; Select the page number style to be used.&lt;br/&gt;&lt;b&gt;Start:&lt;/b&gt; The index within the Style&apos;s range to star at. Eg. If Start=2 and Style=a,b,c, ..., the numbers will begin at b. For the first section in the document this replaces the older First Page Number in the new file window.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Number Out Of Bounds</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The value you have entered is outside the range of page numbers in the current document (%1-%2).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocSectionsBase</name>
    <message>
        <source>Document Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>From</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>To</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="unfinished">Estilo</translation>
    </message>
    <message>
        <source>Start</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="unfinished">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
    <message>
        <source>Shown</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
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
        <source>Name</source>
        <translation type="obsolete">Nombre:</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Valor</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+B</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">B&amp;orrar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>None</source>
        <comment>auto add</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>None</source>
        <comment>types</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>Real Number</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DocumentItemAttributesBase</name>
    <message>
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Valor</translation>
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
        <translation type="unfinished">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">B&amp;orrar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Configuración de la impresora</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Destino de la impresión</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Opciones</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Comando de impresión a&amp;lternativo</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation type="obsolete">Usar un administrador de impresión alternativo, como kprinter
o gtklp, para utilizar opciones de impresión adicionales.</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Co&amp;mando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Imprimir &amp;todo</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Imprimir &amp;página actual</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Imprimir el ran&amp;go</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>N&amp;úmero de copias:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">Impresión &amp;normal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Imprimir &amp;separaciones</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Amarillo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Negro</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Imprimir en colo&amp;r si está disponible</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Imprimir en &amp;escala de grises</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">Opciones A&amp;vanzadas...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Imprimir</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Archivos Postscript (*.ps);; Todos los archivos (*)</translation>
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
        <translation type="unfinished">Página</translation>
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
        <translation type="unfinished">Establecer Tamaño del Medio</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Color</translation>
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
        <translation type="unfinished">Opciones Avanzadas</translation>
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
        <translation type="obsolete">Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp, to utilize additional printing options</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed of cyan, yellow and magenta and using black instead. UCR most affects parts of images which are neutral and/or dark tones which are close to the gray. Use of this may improve printing some images and some experimentation and testing is need on a case by case basis.UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allows you to embed ICC profiles in the print stream when color management is enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables you to explicitely set the media size of the PostScript file. Not recommended unless requested by your printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="unfinished">No mostrar objetos fuera de los margenes en la página imprimida o el archivo exportado</translation>
    </message>
    <message>
        <source>Failed to retrieve printer settings</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Error importando el archivo:
%1</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Error fatal</translation>
    </message>
    <message>
        <source>Converting Image:
%1
failed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error</source>
        <translation type="unfinished">Error</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="obsolete">Editando la macro: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Administrador de Macros</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="obsolete">El archivo &apos;%1&apos; ya existe. 
¿Está seguro de que desa sobreescribirlo?</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="obsolete">Esta macro ha cambiado.
¿Está seguro de que desea descartar todos los cambios?</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="obsolete">A continuación se muestra una traza completa: 

%1</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="obsolete">La compilación de la macro falló, por lo que no 
se pudo salvar en su forma actual. El error fue: 
%1</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Nueva Macro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Esto es el cuadro de diálogo de Editar Macro / Nueva Macro. Aquí puedes cambiar el código fuente de las macros. Edita el código fuente de la macro en el área de edición de texto debajo de la etiqueta &quot;Código Fuente&quot; y pulsa OK para salvar los cambios realizados a la macro.&lt;/qt&gt;
</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="obsolete">Código Fuente:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="obsolete">Editar Macro:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="obsolete">El nombre de la macro que se está editando.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Este objeto muestra el nombre de la macro que estás editando.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Descartar todos los cambios y salir.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Salir del diálogo de edición, descartando todos los cambios realizados. Si desea salir sin guardar los cambios pero no quiere perderlos, guarde los cambios con &quot;Guardar Código Como...&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Guardar los cambios y salir.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Guardar los cambios de la macro y salir. Si hay algún problema con la macro, se mostrará un mensaje y el diálogo de edición no se cerrará.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Este área de texto contiene el código fuente de la macro. Si estás creando una macro nueva no habrá nada, y si estás editando una macro ya existente se mostrará el código fuente de la macro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="obsolete">&amp;Cargar Código ...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Reemplazar el código fuente actual con el código de un archivo.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Cargar el código fuente de &quot;file&quot; en el área de edición. Se reemplazará lo que haya en el área de edición. El código cargado será una macro de Scribus. Si cargas cualquier otro script, deberás editarlo para que funcione como una macro del scripter antes de guardarlo.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="obsolete">&amp;Guardar Código Como...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+G</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Guardar el código fuente editado a un archivo.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="obsolete">Guarda el código fuente - el texto - de la macro a un archivo. Puedes editar el código guardado y cargarlo de nuevo &quot;Cargar Código...&quot;.</translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation type="obsolete">Código fuente de Python (*.py)</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Diálogo Guardar Archivo</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation type="obsolete">Guardar el código de la macro</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Diálogo Abrir Archivo</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation type="obsolete">Selecciona el archivo de código a cargar</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nombre:</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Carácter</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Fuente:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Efectos:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">Al&amp;ineación:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">Letra ca&amp;pital</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Lineas:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Color de &amp;relleno:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Color de tra&amp;zo:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Espaciado vertical</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">A&amp;justar a las líneas de base</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Int&amp;erlineado:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">Arri&amp;ba:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">&amp;Debajo:</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabuladores y sangría</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Nombre del estilo de párrafo</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fuente del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color de relleno del texto</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Proporciona una primera letra de párrafo de mayor tamaño. Se usa para efecto estilístico.</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Determina la altura general, en números de línea, de las letras capitales.</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Alinear texto a las líneas de base</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Espaciado por encima del párrafo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Espacio por debajo del párrafo</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Espacio entre líneas</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation type="obsolete">El nombre del estilo ya existe</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Distancias</translation>
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
    <message>
        <source>Name of the style is not unique</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Fondo</translation>
    </message>
    <message>
        <source>Select for easier reading of light coloured text styles</source>
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
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Guardar y Salir</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Salir sin Guardar</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Nombres de los campos</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Javascripts (*.js);; Todos los archivos (*)</translation>
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
        <translation type="unfinished">Color:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Saturación:</translation>
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
        <translation type="unfinished">Invertir</translation>
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
        <translation type="unfinished">Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Elige un Directorio donde Exportar</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportar como Imagen(es)</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportar al Directorio</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>&amp;Tipo de Imagen:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>Cali&amp;dad:</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>Resol&amp;ución:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> ppp</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Página &amp;Actual</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>Todas las &amp;Páginas</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Intervalo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportar un intervalo de páginas</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportar todas las páginas</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportar sólo la página actual</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolución de las imágenes
Usa 72 ppp para imágenes dirigidas a verse en pantalla</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Tamaño de las imágenes. 100% para que no cambie, 200% para el doble de grande, etc.</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>La calidad de las imágenes - 100% es la  mejor calidad, 1% la peor</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formatos de exportación disponibles</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>El directorio de salida - donde se almacenarán las imágenes.
El nombre del archivo será &apos;documento-página.tipo&apos;</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Cambiar el directorio de salida</translation>
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
        <translation type="unfinished">Normal</translation>
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
        <translation type="unfinished">Saturación</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Color</translation>
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
        <translation type="unfinished">Disolver</translation>
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
        <translation type="unfinished">Opacidad:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Fondo</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="unfinished">Capas</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Rutas</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Sin título</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconocido</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Documento de Scribus</translation>
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
        <translation type="obsolete">Colores</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation type="obsolete">Borrar no &amp;usados</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation type="obsolete">Conjuntos de Colores</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation type="obsolete">Conjunto actual:</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation type="obsolete">&amp;Guardar Conjunto de Colores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation type="obsolete">Elige el conjunto de colores a cargar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation type="obsolete">Guarda el conjunto de colores actual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation type="obsolete">Eliminar del conjunto de colores del documento los que no se usen</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Añadir colores de un documento existente al conjunto actual</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation type="obsolete">Crear un nuevo color en el actual conjunto</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation type="obsolete">Editar el color seleccionado</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation type="obsolete">Hacer una copia del color seleccionado</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation type="obsolete">Borrar el color seleccionado</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation type="obsolete">Establecer el conjunto actual como el conjunto de colores por defecto</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Nombre:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation type="obsolete">Elige un nombre</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Copia de %1</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation type="obsolete">Nuevo color</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importar</translation>
    </message>
</context>
<context>
    <name>FileLoader</name>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Configuraciones globales de las fuentes</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Fuentes Disponibles</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Sustituciones de Fuentes</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Rutas Adicionales</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Sí</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nombre de fuente</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Usar fuente</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Empotrar en:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Subconjunto</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Ruta al archivo de la fuente</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Fuentes &amp;Disponibles</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Reemplazo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Sustituciones de Fuentes</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>A&amp;ñadir...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Rutas Adicionales</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Nombre de fuente</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished">Usar fuente</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished">Empotrar en:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Subconjunto</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished">Ruta al archivo de la fuente</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Previsualizar Fuentes</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Añadir fuente seleccionada al menú Estilo, Fuente</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Salir de la previsualización</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation type="obsolete">Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="obsolete">Nombre de fuente</translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="obsolete">Subconjunto</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="obsolete">Previsualizar Fuentes</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="obsolete">&amp;Buscar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="obsolete">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Añadir fuente seleccionada al menú Estilo, Fuente</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Salir de la previsualización</translation>
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
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
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
        <translation type="obsolete">A&amp;ñadir</translation>
    </message>
    <message>
        <source>Sample will be shown after key release</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreviewBase</name>
    <message>
        <source>Fonts Preview</source>
        <translation type="unfinished">Previsualizar Fuentes</translation>
    </message>
    <message>
        <source>&amp;Quick Search:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Buscar</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation type="unfinished">Nombre de fuente</translation>
    </message>
    <message>
        <source>Doc</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tipo</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="unfinished">Subconjunto</translation>
    </message>
    <message>
        <source>Access</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="unfinished">Texto</translation>
    </message>
    <message>
        <source>Sample text to display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Se&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reset the text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="unfinished">A&amp;ñadir</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
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
        <translation>Posición:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Aquí puedes añadir, cambiar o eliminar límites de colores.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Administrar Guías</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Guías horizontales</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>A&amp;ñadir...</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Guías verticales</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>Aña&amp;dir...</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>B&amp;loquear las guías</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
        <translation type="unfinished">&amp;Página</translation>
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
        <translation type="unfinished">&amp;Cerrar</translation>
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
    <message>
        <source>&amp;Apply to All Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Guide</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="unfinished">Previsualizar</translation>
    </message>
    <message>
        <source>There is empty (0.0) guide already</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Contents</source>
        <translation>Contenidos</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Ayuda de Scribus en línea</translation>
    </message>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Lo siento, no hay ningún manual disponible. Por favor mira: http://docs.scribus.net para documentación actualizada y www.scribus.net para descargas.</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Contenidos</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Buscar</translation>
    </message>
    <message>
        <source>S&amp;earch</source>
        <translation type="obsolete">B&amp;uscar</translation>
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
        <translation type="obsolete">desconocido</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Término de búsqueda:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
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
        <translation type="unfinished">&amp;Imprimir...</translation>
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
        <translation type="unfinished">&amp;Archivo</translation>
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
        <translation type="unfinished">&amp;Editar</translation>
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
        <translation type="unfinished">&amp;Marcadores</translation>
    </message>
    <message>
        <source>Relevance</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Posible División</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Saltar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Configuraciones del Divisor de Palabras</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Sugerencias de &amp;División</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Dividir Texto &amp;Automáticamente Mientras se Escribe</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>Palabra más &amp;pequeña:</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Divisiones Consecutivas Per&amp;mitidas:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Cuando use la opción Extras, Dividir en Sílabas, se mostrará para cada palabra un cuadro de diálogo con las posibles divisiones.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Activa la división silábica automática mientras escribe.</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Longitud mínima que debe tener una palabra para dividirla.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Máximo número de divisiones seguidas.
Un valor de 0 significa divisiones ilimitadas.</translation>
    </message>
</context>
<context>
    <name>ImageInfoDialog</name>
    <message>
        <source>Image Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>General Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Date / Time:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Embedded Profile:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
    <message>
        <source>Profile Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Embedded Paths:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Has Layers:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EXIF Info</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Artist:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User Comment:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Camera Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Camera Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scanner Model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scanner Manufacturer:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished">Importar &amp;EPS/PS...</translation>
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
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDF</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Insertar página</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Insertar</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>página(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>al final</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Insertando</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">&amp;Plantilla (página izquierda):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Plantilla:</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Plantilla (página derecha):</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Insertar</translation>
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
        <translation type="unfinished">Formato de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Apaisado</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">Al&amp;tura:</translation>
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
        <translation>Insertar tabla</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Número de filas:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Número de columnas:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
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
        <source>Edit JavaScripts</source>
        <translation>Editar JavaScripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Editar...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Añadir...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Nuevo Script:</translation>
    </message>
    <message>
        <source>New Script</source>
        <translation>Nuevo Script</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation type="obsolete">¿Realmente desea borrar este Script?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>Do you really want to delete this script?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adds a new Script, predefines a function with the same name. If you want to use this script as an &quot;Open Action&quot; script be sure not to change the name of the function.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Configurar accesos rápidos</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Acción</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Tecla Actual</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Acceso rápido para la acción seleccionada</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Ninguno</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Definido por el &amp;usuario</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Definir</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
        <translation>Mays</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Mays+</translation>
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
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation type="obsolete">Ésta combinación de teclas ya está en uso</translation>
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
    <message>
        <source>This key sequence is already in use</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Meta+</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Capas</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Añadir una nueva Capa</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Borrar Capa</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Subir Capa</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Bajar Capa</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Nueva Capa</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">¿Desea borrar también todos los objetos de esta capa?</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
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
        <translation>Editar Estilos de Línea</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">¿Realmente desea borrar este Estilo?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LoadSavePlugin</name>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Todos los archivos (*)</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>MSpinBox</name>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
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
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="obsolete">El objeto pasado no se puede llamar</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="obsolete">Administrar Macros</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="obsolete">Muestra una ventana para crear, borrar, editar, guardar y cargar macros.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="obsolete">Crear, editar y borrar macros</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="obsolete">&amp;Macro</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Macro Manager</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="obsolete">No puedo abrir el archivo solicitado: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="obsolete">Scribus - Editar Macro</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;El nombre de la macro solicitada ya está elegido por otra macro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;La creación de la macro falló. El administrador de macros no pudo configurar la macro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">La macro &apos;%1&apos; ha informado de un error menor. 
El error es :%2 
A continuación se muestra una traza completa: 

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">La macro &apos;%1&apos; no se ha ejecutado correctamente.
El error es :%2 
A continuación se muestra una traza completa: 

%3
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Nueva Macro</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation type="obsolete">Introduce el nombre de la nueva macro:</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Administrador de Macro</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="obsolete">Renombrar la macro falló porque el nombre ya se está usando.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="obsolete">Scribus - Administrar Macros</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Esta ventana es el Administrador de Macros de Scribus. Aquí puedes crear macros, editar macros, etc. Todos los cambios se realizan usando los botones de la derecha.&lt;/p&gt; 
&lt;p&gt;Todos los cambios realizados en este diálogo tienen efecto instantáneamente - no puedes cancelar las acciones que haces aquí. La tabla del centro muestra las macros actualmente cargadas y algo de información sobre ellas. Usa &quot;What&apos;s This&quot; en la tabla para más información.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuevo</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Crear una nueva macro.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Crear una nueva macro preguntando el nombre de la macro y abriendo el cuadro de diálogo de edición.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="obsolete">Macro</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Editable</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="obsolete">Acceso rápido</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Descripción</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Está tabla muestra las macros que actualmente están definidas.&lt;/p&gt; 

&lt;p&gt;&lt;b&gt;Macro:&lt;/b&gt; El nombre de la macro, como se muestra en la barra de menús y en otros lugares de Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Editable:&lt;/b&gt; Si la macro se puede editar, en este columna aparecerá &quot;Sí&quot;. Normalmente si una macro no se puede editar fue creada con el comando register_macro en un script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Acceso rápido:&lt;/b&gt; La secuencia de acceso rápido asociada con la macro, si tiene. Por ejemplo, CTRL-F8 significa que puedes presionar Control-F8 desde Scribus para ejecutar la macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Descripción:&lt;/b&gt; Si la macro contiene un &quot;docstring&quot;, una cadena especial al principio de su definición que la describe, que se muestra aquí. Si la descripción es larga, sólo se muestra el principio - usa &quot;What&apos;s This&quot; en la entrada de la macro en el menú Macro para ver la descripción completa.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="obsolete">Reno&amp;mbrar</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="obsolete">Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="obsolete">Renombra la macro seleccionada</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Renombra la macro seleccionada. Se te preguntará el nuevo nombre.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Editar...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Editar el código de la macro seleccionado, si está disponible.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+B</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Borrar la macro seleccionada actualmente.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Borrar la macro seleccionada. Es instántaneo y no hay ningún modo de recuperar la macro una vez borrada. Si la macro se crea por un script de inicio, reaparecerá la próxima de vez que se arranque Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="obsolete">E&amp;stablecer Acceso Rápido</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Asigna un acceso rápido para la macro seleccionada.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Asigna un acceso rápido para la macro seleccionada. Se te preguntará el nuevo acceso rápido en un cuadro de diálogo.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="obsolete">E&amp;xportar</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="obsolete">Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="obsolete">Exporta las macros a un archivo</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Exportar macros a un archivo externo. El archvio será un script de Python que contendrá los comandos de scripter para recrear las macros. Se puede ejecutar usando &lt;tt&gt;Cargar Script de Extensión&lt;/tt&gt; desde el menú &lt;tt&gt;Script&lt;/tt&gt;, o el botón importar en el administrador de macros.&lt;/p&gt;
&lt;p&gt;Si quieres una versión legible por humanos de tus macros, selecciona la macro que quieres y presiona el botón &lt;tt&gt;Edit&lt;/tt&gt; y usa el botón &lt;tt&gt;Guardar código&lt;/tt&gt; en el diálogo &lt;tt&gt;Editar Macro&lt;/tt&gt;. No podrás cargar esa versión con &lt;tt&gt;Cargar Script de Extensión&lt;/tt&gt; - sin embargo, podrás crear una nueva macro con el botón &lt;tt&gt;Nuevo&lt;/tt&gt; y usar &lt;tt&gt;Cargar código&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="obsolete">Borrar &amp;Todas</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+T</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="obsolete">Borra todas las macros</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Borra todas las macros registradas. Es instántaneo y no hay ningún modo de recuperar las macros borradas. 
Cualesquiera macros que se creen mediante un script de inicio reaparecerán la próxima vez que se inicie Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importar</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="obsolete">Importa las macros de un archivo.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Carga las macros de un archivo externo.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation type="obsolete">Cerrar este diálogo</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation type="obsolete">Volver a Scribus</translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Editar la macro seleccionada.&lt;/p&gt;
&lt;p&gt;Si este botón esta desactivado, o no hay ninguna macro seleccionada o el administrador de macros no tiene el código fuente de la macro que has seleccionado (en cuyo caso se mostrará &lt;tt&gt;No&lt;/tt&gt; en la columna &lt;tt&gt;Editable&lt;/tt&gt;).&lt;/p&gt;
&lt;p&gt;Si Scribus no tiene el código fuente, probablemente la macro la creó un script.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation type="obsolete">Archivos de Macro de Scribus (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Diálogo de Abrir Archivo</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation type="obsolete">Selecciona el archivo de macro a cargar.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Diálogo de Guardar Archivo</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation type="obsolete">Guardar todas las macros</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation type="obsolete">Scribus - Renombrar Macro</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation type="obsolete">Introduce el nuevo nombre: </translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation type="obsolete">Scribus - Asignar Acceso Rápido</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation type="obsolete">Introduce el nuevo acceso rápido:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">No</translation>
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
        <translation type="unfinished">Formato de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Apaisado</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">Al&amp;tura:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="unfinished">Tipo:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Guías de los márgenes</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">&amp;Exterior:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the margin changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page. If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printer Margins...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import the margins for the selected page size from the available printers.</source>
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
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Copia de %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Copia #%1 de </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Duplicate the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Add a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicación múltiple</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Número de copias:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Desplazamiento &amp;horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Desplazamiento &amp;vertical:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Distancias</translation>
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
        <translation>Ángulo:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Longitud:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">pt</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Importar Plantilla</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar Página(s)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">Desde el documento:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Cambiar...</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Importar página(s):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> de 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Crear página(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">al final</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> de %1</translation>
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
        <translation type="unfinished">&amp;Importar</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Fuente ausente</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">La fuente %1 no está instalada.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Usar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">en su lugar.</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Mover Páginas</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copiar Página</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Mover página(s):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation type="obsolete">hasta:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">antes de la página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">después de la página</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">al final</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
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
    <message>
        <source>To:</source>
        <translation type="unfinished">Hasta:</translation>
    </message>
    <message>
        <source>Number of copies:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Propiedades</translation>
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
        <translation>&amp;Texto</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Imagen</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linea</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Colores</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometría</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotación:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Punto base:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nivel</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editar forma...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Esquinas &amp;redondeadas:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distancia del texto:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Es&amp;pacio:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>Iz&amp;quierda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Derecha:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>Ta&amp;buladores...</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Propiedades del Texto sobre Trazado</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Mostrar curva</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Desplazamiento inicial:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distancia desde curva:</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>El texto flu&amp;ye alrededor del marco</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Usar cuadro circ&amp;unscrito</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Usar línea de c&amp;ontorno</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">Ta&amp;maño de fuente:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Saturación:</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Espaciado personalizado</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Es&amp;pacio entre lineas:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>&amp;Estilo:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>I&amp;dioma:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Escalado Li&amp;bre</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Esc&amp;ala en X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Escalar en Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Escalar al tamaño del &amp;marco</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Proporcional</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Perfil de entrada:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Modo de Visualización:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptivo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Punto izquierdo</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Extremos</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>P&amp;unto base:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>Ti&amp;po de línea:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Ancho de linea:</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Bordes:</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Extremos:</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Sin estilo</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Bordes de celda</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Borde superior</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Borde izquierdo</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Borde derecho</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Borde inferior</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Nombre del objeto seleccionado</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Posición horizontal del punto base actual</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Posición vertical del punto base actual</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Anchura</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Altura</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotación del objeto sobre el punto base actual</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Punto de referencia para las medidas o ángulos de rotación</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Seleccionar la esquina superior izquierda como punto base</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Seleccionar la esquina superior derecha como punto base</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Seleccionar la esquina inferior izquierda como punto base</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Seleccionar la esquina inferior derecha como punto base</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Seleccionar el centro como punto base</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Subir un nivel</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Bajar un nivel</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Traer al frente</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Envíar al fondo</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indica en qué nivel está el objeto, 0 significa que está debajo</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Bloquear o desbloquear el objeto</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Bloquear o desbloquear el tamaño del objeto</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Activar o desactivar la impresión del objeto</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Hacer que el texto en los marcos de debajo fluya alrededor de la forma del objeto</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Usar el cuadro circunscrito en vez de la forma del marco para que fluya el texto</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Usar una segund línea basada originalmente en la forma del marco para que fluya el texto</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fuente del texto u objeto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar la anchura de los caracteres</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation type="obsolete">Color del trazo del texto</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation type="obsolete">Color de relleno del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color de trazo</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color de relleno</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Escritura de Izquierda a Derecha</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Espacio entre líneas</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Idioma del marco para la división de palabras</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Cambiar configuraciones para el punto izquierdo o los extremos</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Patrón de línea</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Grosor de línea</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipo de uniones de las líneas</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipo de extremos</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Estilo de línea del objeto</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Elegir la forma del marco...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editar la forma del marco...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Definir el radio de las esquinas redondeadas</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Número de columnas de texto del marco</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Cambia entre espacio o ancho de las columnas</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Distancia entre columnas</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distancia del texto desde el extremo superior</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distancia del texto desde el extremo inferior</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distancia del texto desde el extremo izquierdo</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distancia del texto desde el extremo derecho</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editar las configuraciones de los tabuladores...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Permite que la imagen sea de un tamaño distinto al del marco</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Desplazamiento horizontal de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Desplazamiento vertical de la imagen dentro del marco</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionar la imagen horizontalmente</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionar la imagen verticalmente</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Mantener la escala en X e Y igual</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Mantener la proporción</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Hacer que la imagen encaje dentro del marco</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Usar proporciones de la imagen en vez de las del marco</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Perfil de entrada de la imagen</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Modo de representación de la imagen</translation>
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
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Anchura de las columnas</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation type="obsolete">E&amp;spacio entre caracteres:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Espacio entre caracteres manual</translation>
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
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text stroke. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color of text fill. Only available with &quot;outline&quot; text decoration.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fill Rule</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Even-Odd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Non Zero</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Plano</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Cuadrado</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Redondeado</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Unión de inglete</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Unión biselada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Unión redondeada</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Ancho de linea:</translation>
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
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source> pt </source>
        <translation> pt </translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Línea sólida</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Línea a trazos</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Línea a puntos</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Línea raya punto</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Línea raya punto punto</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.&lt;br/&gt;Please choose another.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MultiProgressDialogBase</name>
    <message>
        <source>Progress</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Overall Progress:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Editar Plantillas</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuevo</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">¿Realmente desea borrar esta Plantilla?</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;No</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Nombre:</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Nueva Plantilla</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Copia de %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Nombre:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Copia #%1 de </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
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
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Nuevo documento</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Libro de contabilidad</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloide</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Personalizado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">Primero pá&amp;gina izquierda</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">Iz&amp;quierda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Derecha:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">I&amp;nferior:</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Núme&amp;ro de la primera página:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>&amp;Unidad por defecto:</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milímetros (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Picas (p)</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Marcos de te&amp;xto automáticos</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Columnas</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>Es&amp;pacio:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Tamaño de página del documento, elige un tamaño estándar o uno personalizado</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientación de las páginas del documento</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas, editable si se elige un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas, editable si se elige un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Hacer la primera página del documento la página izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation type="obsolete">Número de la primera página del documento</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medida por defecto</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Crear marcos de texto automáticamente cuando se añaden nuevas páginas</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Número de columnas de los marcos de texto creados automáticamente</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Distancia entre columnas de los marcos de texto creados automáticamente</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Exterior:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Ejecutivo</translation>
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
        <source>All Files (*)</source>
        <translation type="obsolete">Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Abrir</translation>
    </message>
    <message>
        <source>&amp;New Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open &amp;Existing Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Recent &amp;Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="unfinished">Nuevo &amp;desde Plantilla...</translation>
    </message>
    <message>
        <source>Load documents with predefined layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Start a document from a template made by other users or yourself (f.e. for documents you have a constant style).</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Página izquierda</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Página derecha</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Nodos</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Coordenadas &amp;Absolutas</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Posición &amp;X:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Posicón &amp;Y:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Editar &amp;contorno</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Fin de edición</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Mover nodos</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Mover puntos de control</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Añadir nodos</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Borrar nodos</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Mover puntos de control independientemente</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Mover puntos de control simétricamente</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Reiniciar puntos de control</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Reinicia este punto de control</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Abrir un polígono o cortar una curva de Bézier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Cierra esta curva de Bézier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Voltear horizontalmente</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Voltear verticalmente</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Cizallar horizontalmente a la derecha</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Cizallar horizontalmente a la izquierda</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Cizallar verticalmente hacia arriba</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Cizallar verticalmente hacia abajo</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotar en el sentido contrario a las agujas del reloj</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotar en el sentido de las agujas del reloj</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation type="obsolete">Reducir el tamaño en el porcentaje mostrado</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Agrandar el tamaño en el porcentaje mostrado</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Ángulo de rotación</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation type="obsolete">Porcentaje en que agrandar o reducir</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Activar el modo de edición del contorno</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Reiniciar el contorno a la forma original del marco</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Cuando está marcada usa coordenadas relativas a la página,
de otro modo las coordenadas son relativas al objeto.</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shrink the Size of the Path by shown %</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by the shown value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>% to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value to Enlarge or Shrink By</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODPlug</name>
    <message>
        <source>This document does not seem to be an OpenOffice Draw file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished">Importar &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice.org Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most OpenOffice.org Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenDocument 1.0 Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org 1.x Draw</source>
        <comment>Import/export format name</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Usa nombre del documento como un prefijo para los estilos de párrafo</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>No preguntar de nuevo</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Opciones de importación de OpenDocument</translation>
    </message>
    <message>
        <source>Update paragraph Styles</source>
        <translation type="obsolete">Actualizar Estilos de Párrafo</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Activar esto sobreescribirá los estilos existentes en el documento actual</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionar Estilos de Párrafo</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionar estilos de párrafo por atributos. Esto resultará en menos estilos de párrafo similares, retendrá los atributos de los estilos, incluso si los estilos del documento original se llamaban de forma diferente.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Anteponer el nombre del documento al estilo de párrafo de Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Establecer estas opciones por defecto y no preguntar de nuevo cuando se importe un OASIS OpenDocument.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
</context>
<context>
    <name>OldScribusFormat</name>
    <message>
        <source>Scribus Document</source>
        <translation type="unfinished">Documento de Scribus</translation>
    </message>
    <message>
        <source>Scribus 1.2.x Document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
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
        <translation type="obsolete">Consola de Script</translation>
    </message>
</context>
<context>
    <name>PDFExportDialog</name>
    <message>
        <source>Save as PDF</source>
        <translation type="unfinished">Guardar como PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="unfinished">&amp;Archivo de salida:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Guardar como</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="unfinished">Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commercial printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>O&amp;utput to File:</source>
        <translation type="obsolete">&amp;Archivo de salida:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation type="obsolete">Cam&amp;biar...</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Rango de exportación</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Todas las páginas</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Rango</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotación:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Opciones del Archivo</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">Compat&amp;ibilidad:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">E&amp;ncuadernación:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Margen Izquierdo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Margen Derecho</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Generar &amp;miniaturas</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Guardar marcos enla&amp;zados como artículos PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">Incluir &amp;marcadores</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete"> ppp</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">Resol&amp;ución:</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Comprimir Texto y Gráficos &amp;Vectoriales</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Configuración de imágenes</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Automático</translation>
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
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">Mét&amp;odo:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">Cali&amp;dad:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Máxima</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Alta</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Media</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Baja</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Mínima</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Reba&amp;jar las imágenes a:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">G&amp;eneral</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Empotrar todas las fuentes</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Empotrar &amp;subconjunto de todas las fuentes</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Empotrar</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Fuentes Disponibles:</translation>
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
        <translation type="obsolete">Fuentes a empotrar:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Fuentes a empotrar un subconjunto:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Fuentes</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Acti&amp;var Efectos de Presentación</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Página</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">&amp;Mostrar vista previa</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Efectos</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">Duración de la vis&amp;ualización:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Duración del efect&amp;o:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">&amp;Tipo de efecto:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">Lí&amp;neas:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">&amp;Desde:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">Di&amp;rección:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete"> seg</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Sin efecto</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Bandas</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Caja</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Disolver</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Brillo</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">División</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Cortina</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Vertical</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Interior</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Exterior</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Izquierda a Derecha</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Arriba a Abajo</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Abajo a Arriba</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Derecha a Izquierda</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Arriba-Izda a Abajo-Dcha</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">Apl&amp;icar efectos en todas las páginas</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Presentación</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">Usar cif&amp;rado</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Contraseñas</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Usuario:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">&amp;Propietario:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Configuraciones</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Permitir &amp;imprimir el documento</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Permitir ca&amp;mbiar el documento</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Permitir c&amp;opiar texto y gráficos</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Permitir a&amp;ñadir anotaciones y campos</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">&amp;Seguridad</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">General</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">Salida &amp;dirigida a:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Pantalla / Internet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Impresora</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">&amp;Usar configuraciones de renderizado personalizadas</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Configuraciones de Renderizado</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">F&amp;recuencia:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">Á&amp;ngulo:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">Función de &amp;punto:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Punto simple</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Línea</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Redondeado</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Elipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Colores Sólidos:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Usar perfil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Perfil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Modo de Visualización:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Perceptivo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Saturación</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Imágenes:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">No usar perfiles ICC empotrados</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">Co&amp;lor</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Modo de salida de PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">Texto &amp;informativo:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">&amp;Perfil de salida:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Cuadro de recorte</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Exportar todas las páginas a PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Exportar un rango de páginas a PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Determina la compatibilidad del PDF. La opción por defecto es Acrobat 4.0 que da la mayor compatibilidad.
Elige Acrobat 5.0 si tu archivo tiene características de PDF 1.4, como transparencia, o si requieres cifrado de 128 bits.
PDF/X-3 es para exportar PDF para impresión profesional y se puede seleccionar si tienes activados los ajustes de color.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Determinar la encuadernación de las páginas en el PDF. Si no sabes
que elegir deja la opción por defecto - Izquierda.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Genera miniaturas de cada página en el PDF.
Algunos visores pueden usar las miniaturas para la navegación.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Generar artículos en el PDF, lo que es útil para navegar artículos enlazados en un PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Empotrar los marcadores que has creado en tu documento.
Son útils para navegar en documentos PDF largos.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Resolución del testo y gráficos vectoriales exportados.
Esto no afecta a la resolución de las imágenes como las fotos.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Compresión de texto y gráficos.
Si no tienes una razón para no usarlo, deja esta opción.
Reduce el tamaño del PDF.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Método de compresión para las imágenes.
Automático permite a Scribus elegir el mejor método.
ZIP es bueno para imágenes con colores sólidos.
JPEG crea archivos PDF más pequeños que tienen muchas fotos
(posiblemente con una leve pérdida de calidad).
Dejalo puesto a automático, excpeto si necesitas opciones de compresión especiales.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Niveles de compresión: Mínimo (25%), Bajo (50%), Medio (75%), Alto (85%), Máximo (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Rebaja las imágnes a la resolución seleccionada.
Dejar esta opción sin seleccionar las renderizada con su propia resolución.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">PPP (puntos por pulgada) con que exportar las imágenes.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Empotrar las fuentes en el PDF. Esto preservará
el diseño y la apariencia del documento.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Activar los efectos de presentación cuando se usa Acrobat Reader en modo de pantalla completa.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Mostrar la previsualización de cada página listada arriba.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Cuanto tiempo se muestra la página antes de que la presentación empiece en la página actual.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">La duración del efecto.
Cuanto menor tiempo más rápido será el efecto, y viceversa.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Tipo del efecto que se mostrará.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Dirección del movimiento de las líneas para los efectos de división y bandas.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Posición inicial de los efectos de caja y división.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Dirección de los efectos de brillo y cortina.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Aplicar el efecto actual en todas las páginas.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Activar las características de securidad en el PDF.
Si seleccionas Acrobat 4.0, el PDF estará protegido por un cifrado de 40 bits.
Si seleccionas Acrobat 5.0, el PDF estará protegido por un cifrado de 128 bits.
Atención: el cifrado PDF no es tan fiable como el cifrado GPG o PGP y tiene algunas limitaciones.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Elige una contraseña maestra que activará o desactivará todas
las características de seguridad del PDF.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Elige una contraseña para que los usuarios puedan leer el PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Permitir imprimir el PDF, si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Permitir modificar el PDF. Si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Permitir copiar el texto o los gráficos del PDF.
Si no se selecciona no se permtirá.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Modelo de color para la salidad del PDF.
Elige Pantalla / Web para PDFs que se usarán para mostrar en pantalla e imprimir en impresoras de tinta.
Elige Impresora para imprimir en impresoras de 4 colores CMYK.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Esta es una opción avanzada que no se activa por defecto. Sólo se debería activar
cuando lo pida específicamente tu imprenta y te den los detalles exactos que necesitan.
De otra forma, puede que el PDF no se imprima apropiadamente y no será portable entre distintos sistemas.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Empotrar un perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Modo de representación para colores sólidos</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Empotrar un perfil de color para las imágenes</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">No usar los perfiles de color que estén empotrados en las imágenes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Perfil de color para imágenes</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Modos de representación para las imágenes</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Perfil de salida para impresión. Si es posible, pide consejo a tu imprenta sobre la elección del perfil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Cadena obligatoria para PDF/X-3 o el PDF no será conforme a PDF/X-3.
Recomendamos que uses el título del documento.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Distancia del margen desde el extremo superior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Distancia del margen desde el extremo inferior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Distancia del margen desde el extremo izquierdo de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Distancia del margen desde el extremo derecho de la página real</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Voltear página(s) horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Voltear página(s) verticalmente</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Guardar como</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation type="obsolete">Archivos PDF (*.pdf);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation type="obsolete">Guardar como PDF</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Se deben activar los ajustes de color para usar PDF/X-3. Puedes activar los ajustes de color en el menú Preferencias.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt; PDF/X-3 soportado y activado, pero no se puede usar para la versión de pDF seleccionada. Si quieres usar PDF/X-3 necesitas establecer PDF/X-3 como tu versión de PDF (nivel de compatibilidad).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;PDF/X-3 no está soportado por la compilación de Scribus (no hay soporte de CMS).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation type="obsolete">Permitir añadir anotaciones y campos al PDF. 
Si no se selecciona, se impedirá la edición de anotaciones y campos.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation type="obsolete">Opciones de seguridad de PDF</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation type="obsolete">Opciones de PDF/X-3</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;No se puede usar la seguridad de PDF con PDF/X-3. Si quieres activar la seguridad, cambia tu versión de PDF (nivel de compatibilidad) a otra cosa que no sea PDF/X-3.&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>PDFlib</name>
    <message>
        <source>Saving PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Items on Current Page:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Previsualización de la Impresión</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Suavizar &amp;Texto</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Suavizar &amp;Gráficos</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Mostrar Trans&amp;parencia</translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>&amp;Eliminación de Color Subyacente</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>&amp;Mostrar CMYK</translation>
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
        <translation type="obsolete">Proporciona una vista más agradable del texto, al coste de una ligera
ralentización en la previsualización. Sólo afecta a fuentes Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Proporciona una vista más agradable de las fuentes TrueType y OpenType, EPS, PDF y gráficos vectoriales, al coste de una ligera ralentización en la previsualización</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Muestra transparencias y objetos transparentes. Requiere Ghostscript 7.07 o superior</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Da una previsualización de la impresión simulando tintas CMYK genéricas, en vez de colores RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Activar/desactivar la tinta C (Cian)</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Activar/desactivar la tinta M (Magenta)</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Activar/desactivar la tinta Y (Amarillo)</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Activar/desactivar la tinta K (Negro)</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Amarillo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Negro</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="unfinished">Imprimir...</translation>
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
    <message>
        <source>Close</source>
        <translation type="unfinished">Cerrar</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Archivo</translation>
    </message>
</context>
<context>
    <name>PSLib</name>
    <message>
        <source>Processing Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting Pages:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Copiar aquí</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Mover aquí</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Pegar</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Mostrar Má&amp;rgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Mostrar &amp;Marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Mostrar &amp;Imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Mostrar &amp;Cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Mostrar &amp;Guías</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Mostrar Líneas de &amp;Base</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">A&amp;justar a las Guías</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Imagen</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Archivo: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">PPP originales: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">PPP actuales: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Texto enlazado</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Marco de Texto</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Texto sobre trazado</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Párrafos: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Caracteres: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Imprimir: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Activado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Desactivado</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">In&amp;formación</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">Car&amp;gar imagen...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">&amp;Imagen visible</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">Actuali&amp;zar imagen</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Editar imagen</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">&amp;Ajustar marco a imagen</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Car&amp;gar texto...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">&amp;Añadir texto...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Editar texto...</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">&amp;Marcador PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">&amp;Anotación PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">&amp;Propiedades de las Anotaciones</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">&amp;Propiedades de Campo</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Opciones de &amp;PDF</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">&amp;Editar texto...</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Blo&amp;quear</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Desblo&amp;quear</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Bloquear tama&amp;ño</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Desbloquear tama&amp;ño</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Enviar al Álbu&amp;m de Recortes</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">E&amp;nviar a Capa</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Insertar texto de e&amp;jemplo</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">Agr&amp;upar</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">Desagr&amp;upar</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Nive&amp;l</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">&amp;Enviar al fondo</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">&amp;Traer al frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Subir</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Marco de &amp;imagen</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">&amp;Polígono</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Contorno</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">Marco de &amp;texto</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">Curva de &amp;Bézier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">Con&amp;vertir a</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">B&amp;orrar contenidos</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Mostrar p&amp;ropiedades...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Ocultar p&amp;ropiedades...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">¿Realmente desea borrar todo el texto?</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">El programa</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">no se encuentra</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Imagen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Línea</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polilínea</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>TextoTrazado</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Copia de </translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
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
        <source>Name</source>
        <translation type="obsolete">Nombre:</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="obsolete">Valor</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation type="obsolete">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+B</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">B&amp;orrar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <comment>relationship</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
</context>
<context>
    <name>PageItemAttributesBase</name>
    <message>
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Tipo</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Valor</translation>
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
        <translation type="unfinished">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Copiar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">B&amp;orrar</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancelar</translation>
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
    <name>PagePalette</name>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation type="unfinished">Ordenar Páginas</translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="unfinished">Páginas del Documento:</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Página </translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> de %1</translation>
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
        <translation type="unfinished">Carta</translation>
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
        <translation type="unfinished">Libro de contabilidad</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished">Ejecutivo</translation>
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
        <translation type="unfinished"></translation>
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
        <translation>Resultado</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Resultados de la búsqueda para: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Previsualizar</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Seleccionar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Imágenes</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Ruta</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Estado</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ir a</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Ausente</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Administrar imágenes</translation>
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
    <message>
        <source>Select a base directory for search</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished">Guardar como &amp;Imagen...</translation>
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
        <source>Import</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Importar</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Desconocido</translation>
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
        <translation type="unfinished">Tipo</translation>
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
        <translation type="unfinished">Archivo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">No</translation>
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
        <translation>Propiedades del polígono</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Vértices:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Aplicar &amp;Factor</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">Fac&amp;tor:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Número de vértices de los polígonos.</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Grados de rotación de los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Aplicar el factor convexo/cóncavo para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Con un valor negativo el polígono será cóncavo (o con forma de estrella),
con un valor positivo será convexo</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished">&amp;Vértices:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Aplicar &amp;Factor</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">Fac&amp;tor:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Número de vértices de los polígonos.</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Grados de rotación de los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished">Aplicar el factor convexo/cóncavo para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Con un valor negativo el polígono será cóncavo (o con forma de estrella),
con un valor positivo será convexo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped), a positive value will make it convex</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Preferencias</translation>
    </message>
    <message>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guías</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografía</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Álbum de recortes</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Visualización</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Herramientas Externas</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Varios</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>GUI</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">Tama&amp;ño de fuente:</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Unidades</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Milímetros (mm)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Picas (p)</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Configuración del ratón</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>Desplazamiento de la r&amp;ueda:</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete"> px</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">Radio de s&amp;elección:</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Menús</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>D&amp;ocumentos recientes:</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Rutas</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Documentos:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>Ca&amp;mbiar...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;Perfiles ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Cam&amp;biar...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Camb&amp;iar...</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">P&amp;lantillas:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Cambia&amp;r...</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Folio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Libro de contabilidad</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Tabloide</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientación:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Primero página izquierda</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>D&amp;erecha:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Izquierda:</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Autoguardado</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation type="obsolete">Acti&amp;vado</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>Interva&amp;lo:</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Disposición de la Cuadrícula</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Espacio entre Líneas Me&amp;nores:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Espacio entre Líneas Ma&amp;yores:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Distancia de A&amp;juste a las Guías:</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Colores de la Cuadrícula</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Color de las Líneas M&amp;enores:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Color de las Líneas May&amp;ores:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Color de las Guías del &amp;Usuario:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">Color de las Líneas de &amp;Base:</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Posición:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">Det&amp;rás</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">&amp;Delante</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Líneas de Base</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">Ac&amp;tivadas</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">Desacti&amp;vadas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Subíndice</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">&amp;Desplazamiento</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Escala:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Superíndice</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Des&amp;plazamiento</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">E&amp;scala:</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Versalitas</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">Esca&amp;la:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Otro</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">Líneas de &amp;Base:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Mar&amp;gen de las Líneas de Base:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">&amp;Interlineado automático:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">&amp;Fuente por defecto:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">&amp;Tamaño por defecto:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">C&amp;olor del texto:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">Es&amp;pacio:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Color de &amp;línea:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Saturación:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">Color de &amp;Relleno:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">Sat&amp;uración:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Tipo de Línea</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">A&amp;ncho de linea:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">Mí&amp;nimo</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Má&amp;ximo</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Incremento:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">Escalado Li&amp;bre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">Escalado &amp;Horizontal</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">Escalado &amp;Vertical</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">&amp;Escalar Imagen al Tamaño del Marco</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">&amp;Mantener la Relación de Aspecto</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Color de &amp;relleno:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">&amp;Vértices:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Rotación:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Aplicar &amp;Factor</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">Fac&amp;tor:</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation type="obsolete">Otras Opciones</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation type="obsolete">&amp;Guardar los Contenidos cuando Cambian</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation type="obsolete">Previsualizar</translation>
    </message>
    <message>
        <source>Small</source>
        <translation type="obsolete">Pequeño</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Mediano</translation>
    </message>
    <message>
        <source>Large</source>
        <translation type="obsolete">Grande</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Mostrar &amp;páginas una al lado de otra</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Colores de la Página</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Fondo:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Márgenes:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Mostrar el Área &amp;No Imprimible con el Color del Margen</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Usar las Características de &amp;Transparencia de PDF 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>A&amp;justar Tamaño de la Pantalla</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">Para ajustar la pantalla arrastra el deslizador de abajo y ajusta la regla.</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Intérprete Postscript</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Nombre del Ejecutable:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Suavizar &amp;Texto</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Suavizar &amp;Gráficos</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Herramienta de Procesado de Imágenes</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Nombre del &amp;Ejecutable:</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation type="obsolete">Impresión</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation type="obsolete">Recortar a los &amp;Márgenes de la Página</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation type="obsolete">Aplicar &amp;Eliminación del Color Subyacente</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation type="obsolete">Elige la apariencia y decoración de ventanas por defecto.
Scribus hereda cualquier tema de KDE o Qt disponible.</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Tamaño de fuente por defecto para los menús y ventanas</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidad de medida por defecto</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Número de líneas que Scribus desplazará con cada movimiento de la rueda del ratón</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Radio del área donde Scribus permitirá que se seleccione un objeto</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Número de documentos editados recientemente a mostrar en el menú Archivo</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Directorio de documentos por defecto</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Directorio de scripts por defecto</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Directorio adicional para Plantillas de Documentos</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Tamaño de página por defecto, ya sea un tamaño estándar o uno personalizado</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientación de las páginas por defecto</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Anchura de las páginas, edtable si has elegido un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Altura de las páginas, edtable si has elegido un tamaño de página personalizado</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Hacer que la primera página esté a la izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation type="obsolete">Cuando se activa, Scribus guarda una copia de seguridad del archivo
con la extensión .bak cada vez que pasa el período indicado</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Período de tiempo para el autoguardado</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Distancia entre las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Distancia entre las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Distancia dentro de la que un objeto se ajustará a las guías</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Color de las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Color de las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Color de las guías que se insertan</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Poner la cuadrícula detrás de los objetos de la página</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Poner la cuadrícula delante de los objetos de la página</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Desplazamiento sobre la línea de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Tamaño relativo del superíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Desplazamiento por debajo de la línea base de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Tamaño relativo del subíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Tamaño relativo de las versalitas comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Porcentaje de incremente sobre el tamaño de fuente para el interlineado</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Propiedades de Marco de Texto</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Propiedades de Marco de Imagen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Propiedades de Dibujo de Formas</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Niveles de Zoom por Defecto</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Propiedades de Dibujo de Líneas</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Propiedades de Dibujo de Polígonos</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Tamaño de la fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Color de la fuente</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Número de columnas en un marco de texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Hueco entre las columnas del marco de texto</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Ejemplo de la fuente</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Los marcos de imagen permiten el escalado de la imagen a cualquier tamaño</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Escalado horizontal de las imágenes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Escalado vertical de las imágenes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Mantener iguales los escalados horizontal y vertical</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Las imágenes se escalan al tamaño del marco</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Mantener las proporciones originales de las imágenes escaladas automáticamente</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Color de relleno de los marcos de imagen</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Saturación del color de relleno</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Color de línea de las formas</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Saturación del color de las líneas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Color de relleno de las formas</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Estilo de línea de las formas</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Ancho de línea de las formas</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Reducción mínima permitida</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Ampliación máxima permitida</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Cambio en el nivel de ampliación por cada operación de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Color de las líneas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Saturación del color</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Estilo de las líneas</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Ancho de las líneas</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Número de vértices de los polígonos.</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Grados de rotación de los polígonos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Aplicar el factor convexo/cóncavo para cambiar la forma de los polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Polígono de ejemplo</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Con un valor negativo el polígono será cóncavo (o con forma de estrella),
con un valor positivo será convexo</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation type="obsolete">Elige el tamaño de la previsualización en el album de recortes</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation type="obsolete">Guardar los contenidos del album de recortes después de un cambio</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">Cuando se activa las páginas enfrentadas, muestra las dos páginas juntas</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Color del papel</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Color de las líneas de margen</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Enmascarar el área fuera de los margenes con el color del margen</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Activar las característcias de transparencia con la exportación de PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Establecer el nivel de ampliación de la regla</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Suavizar (antialiasing) texto para la visualización en pantalla de EPS y PDF</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Suavizar (antialiasing) gráficos para la visualización en pantalla de EPS y PDF</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation type="obsolete">No mostrar objetos fuera de los margenes en la página imprimida o el archivo exportado</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Una forma de desactivar algunas de las saturaciones grises que están compuestas
de cian, amarillo y magenta y en lugar de eso usar negro.
UCR afecta en su mayoría a partes de images que son de tonos neutrales u
oscuros cercanos al gris. Esto puede mejorar la impresión de algunas imágenes
y se necesita hacer pruebas en cada caso.
UCR reduce la posibilidad de sobresaturación con tintas CMY (cian, magenta, amarillo).</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Elige un directorio</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Interior:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">&amp;Exterior:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Ejecutivo</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation type="obsolete">Directorio de perfiles ICC por defecto. No se puede 
cambiar con documentos abiertos.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation type="obsolete">Activar las líneas de base</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation type="obsolete">Desactivar las líneas de base</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation type="obsolete">Localización del intérprete Ghostscript</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation type="obsolete">Localización del editor de gráficos. Si usas gimp y tu distro 
lo incluye, recomendamos &apos;gimp-remote&apos;, ya que te permite 
editar la imagen en una instancia de gimp que ya se esté 
ejecutando.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Idioma:</translation>
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
        <translation type="unfinished">Color:</translation>
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
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Archivo</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Sí</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">No</translation>
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
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
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
    <message>
        <source>&amp;Font Size (Menus):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size (&amp;Palettes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks. Scribus inherits any available KDE or Qt themes, if Qt is configured to search KDE plugins.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default font size for the tool windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot be changed with a document open. By default, Scribus will look in the System Directories under Mac OSX and Windows. On Linux and Unix, Scribus will search $home/.color/icc,/usr/share/color/icc and /usr/local/share/color/icc </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension each time the time period elapses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps. If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp and your distro includes it, we recommend &apos;gimp-remote&apos;, as it allows you to edit the image in an already running instance of gimp.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Página izquierda</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Página derecha</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="unfinished">Postscript</translation>
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
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fibonacci</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Golden Mean</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Nine Parts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Gutenberg</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, Gutenberg sets margins classically. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>None</source>
        <comment>layout type</comment>
        <translation type="unfinished">Ninguno</translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Abrir...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Archivo</translation>
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
        <translation type="unfinished">Consola de Script</translation>
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
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Atención</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Open Python Script File</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>T&amp;ono:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>&amp;Sat:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>Va&amp;l:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Rojo:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Verde:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>A&amp;zul:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal al&amp;fa:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>Colores &amp;básicos</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>Colores &amp;propios</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definir colores propios &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Añadir a Colores Propios</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Seleccionar color</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copiar o Mover un Archivo</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nombre del fichero:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tipo de fichero:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Subir un directorio</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamaño</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Mirar &amp;en:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Atrás</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Crear Nueva Carpeta</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Vista de Lista</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Vista Detallada</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Previsualizar Información de Archivos</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Previsualizar Contenidos de Archivos</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Lectura-Escritura</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Sólo Lectura</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Sólo Escritura</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inaccesible</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Enlace a Archivo</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Enlace a Directorio</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Enlace a Especial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Dir</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Especial</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renombrar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>&amp;Recargar</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Ordenar por &amp;Nombre</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Ordenar por &amp;Tamaño</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Ordenar por &amp;Fecha</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Sin ordenar</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Ordenar</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Mostrar archivos &amp;ocultos</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>el archivo</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>el directorio</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>el enlace simbólico</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Borrar %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;¿Está seguro de que desea borrar %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sí</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nuevo Carpeta 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nueva Carpeta</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nueva Carpeta %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Encontrar Directorio</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Directorios</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Guardar</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Error</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Archivo no encontrado.
Comprueba la ruta y el nombre del archivo.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Todos los Archivos (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Selecciona un Directorio</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Directorio:</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Leer: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Escribir: %1</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Fuente</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Estilo de fuente</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efectos</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Tac&amp;hado</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Subrayado</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Color</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Ejemplo</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Escr&amp;itura:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Seleccionar fuente</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Preparar</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Personalizar...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Acerca de Qt&lt;/h3&gt;&lt;p&gt;Este programa usa la versión de Qt %1.&lt;/p&gt;&lt;p&gt;Qt es un conjunto de herramientas para el desarrollo de aplicaciones e interfaces gráficas multiplataforma.&lt;/p&gt;&lt;p&gt;Qt proporciona portabilidad entre MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux y la mayoría de las variantes de Unix.&lt;br&gt;Qt también está disponible para dispositivos empotrados.&lt;/p&gt;&lt;p&gt;Qt es un producto de Trolltech. Ver &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; para más información.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Importing text</source>
        <translation>Importando texto</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanés</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Vasco</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Bulgaro</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasileño</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalán</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chino</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Checo</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Danés</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandés</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglés</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Inglés (Británico)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemán</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandés</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francés</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Gallego</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Griego</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonesio</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Coreano</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Noruego (Bokmål)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Noruego (Nynorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Noruego</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polaco</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Ruso</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Sueco</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Español</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Español (Latinoamérica)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Esloveno</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Serbio</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>La fuente %1 está rota, descartándola</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Iniciando...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atención</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>¿Realmente quieres sobreescribir el archivo:
%1 ?</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">Previsualizar &amp;Fuentes</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Insertar Carácter Especial</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Nuevo &amp;desde Plantilla...</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Plantilla: </translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Boletines</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Folletos</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catálogos</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Cuartillas</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Letreros</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Tarjetas</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Membretes</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Sobres</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Tarjetas profesionales</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendarios</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Etiquetas</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menús</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programas</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formularios PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Presentaciones PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revistas</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Posters</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Anuncios</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Documentos de Texto</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Dípticos</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Cajas de discos</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Plantillas Propias</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Guardar como &amp;Imagen...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Guardar como Imagen</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Se produjo un error escribiendo los archivos de salida.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Se produjo un error escribiendo los archivos de salida.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Exportación realizada con éxito.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>El archivo existe. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>ya exste. ¿Sobreescribir?</translation>
    </message>
    <message>
        <source>No</source>
        <translation>No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sí</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Sí a todo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Previsualización de la Impresión</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Pre&amp;visualizar</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Importar &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Todos los formatos soportados (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Guardar como P&amp;lantilla...</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede obtener un color sin nombre</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Color no encontrado</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede cambiar un color sin nombre</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede crear un color sin nombre</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede borrar un color sin nombre</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>No se puede reemplazar un color sin nombre</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Error abriendo el documento</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Error guardando el documento</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unidad fuera de rango. Usa una de las constantes scribus.UNIT_*</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>El destino no es un marco de imagen.</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="obsolete">Ya existe un objeto con el nombre solicitado</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="obsolete">La lista de puntos debe tener al menos 2 puntos (4 valores)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="obsolete">La lista de puntos debe tener un número par de valores</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="obsolete">La lista de puntos debe tener al menos 3 puntos (6 valores)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="obsolete">La lista de puntos debe tener al menos 4 puntos (8 valores)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="obsolete">La lista de puntos debe tener un múltiplo de 6 valores</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Objeto no encontrado</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Estilo no encontrado</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">No puedo asignar un estilo a un marco que no es de texto</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Error guardando EPS</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="obsolete">Número de página fuera de rango</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">El argumento no es una lista: debe ser una lista de valores float</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">El argumento contiene valores no numéricos: debe ser una lista de valores float</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="obsolete">Ancho de línea fuera de rango, debe ser 0 &lt;= ancho_de_línea &lt;= 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Saturación de la línea fuera de rango, debe ser 0 &lt;= saturación &lt;= 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Saturación del relleno fuera de rango, debe ser 0 &lt;= saturación &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>El radio de las esquinas debe ser un número positivo.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Estilo de línea no encontrado</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño de fuente de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener la fuente de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el tamaño del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el número de columnas de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el interlineado de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener la distancia entre columnas de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede obtener el texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el texto de un marco que no es de texto</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede insertar texto en un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Alineamiento fuera de rango. Usa una de las constantes scribus.ALIGN*.</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Índice de selección fuera de rango</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Se intentó establecer el progreso mayor que el máximo</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">Manual de S&amp;cripter...</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Referencia en línea</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Guardar la página como &amp;SVG...</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg *.svgz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Imágenes SVG (*.svg);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Importar &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="obsolete">Importar &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation type="obsolete">OpenOffice.org Draw (*.sxd);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Archivos de Valores Separados por Coma</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>datos_CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>cabecera_CSV</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Archivos HTML</translation>
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
Enlaces externos</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Documentos de OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtros de Texto</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Archivos de texto</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Color no encontrado - error de python</translation>
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
        <translation type="obsolete">Módulo de interfaz de Python para Scribus 

Este módulo es la interfaz de Python para Scribus. Proporciona funciones 
para controlar scribus y manipular objetos del área de trabajo. Cada 
función está documentada individualmente más abajo. 

Hay unas pocas cosas comunes a lo largo de la interfaz. 

La mayoría de las funciones operan en marcos. Los marcos se identifican 
por su nombre, una cadena - no son objetos reales de Python. Muchas 
funciones toman un parámetro opcional (sin palabra clave), el nombre de 
un marco. 
Hay muchas excepciones comunes a la mayoría de las funciones. Estás no 
están documentadas en la documentación de cada función. 
- Muchas funciones lanzarán NoDocOpenError si se intenta usarlas sin un documento sobre el que trabajar. 
- Si no se pasa un nombre de marco a una función que lo requiere, la función usará el marco que esté seleccionado actualmente, si lo hay, o lanzará NoValidObjectError si no puede encontrar nada sobre lo que trabajar. 
- Muchas funciones lanzarán WrongFrameTypeError si se intenta usarlas en un tipo de marco con el que no tiene sentido. Por ejemplo, establecer el color de texto en un marco de gráficos no tiene sentido, y provocará que se lance esta excepción. 
- Los errores resultantes de llamadas al API de Python se pasarán sin alterar. Así que la lista de excepciones lanzadas por cualquier función como se proporciona aquí y en su documentación está incompleta. 

Los detalles de que excepciones puede lanzar cada función se proporcionan 
en la documentación de la función.</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuración personalizada (opcional): </translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Configuración estándar:</translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Palabras Cortas...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Palabras Cortas está procesando. Espere por favor...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Procesamiento de Palabras Cortas realizado.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Afrikaans</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Turco</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Ucraniano</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Galés</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>El nombre de archivo debe ser una cadena.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>No se pueden borrar las configuraciones de tipo de imagen.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>El tipo de imagen debe ser una cadena.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>El atributo &apos;allTypes&apos; es de sólo lectura</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Falló exportando una imagen</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Color no encontrado.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Color no encontrado en el documento.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Color no encontrado en los colores por defecto.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>No puedo escalar a 0%.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>El objeto especificado no es un marco de imagen.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Fuente no encontrada.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>No se puedo visualizar un ejemplo vacío.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>No se puede tener una capa sin nombre.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>Capa no encontrada.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>No se puede eliminar la última capa.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>No se puede crear una capa sin nombre.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Índice de inserción fuera de rango.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer la alineación del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Tamaño de fuente fuera de rango - debe ser 1 &lt;= tamaño &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el tamaño de fuente de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer la fuente de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Interlineado fuera de rango, debe ser &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el interlineado de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Distancia entre columnas fuera de rango, debe ser positiva.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer la distancia entre columnas de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Número de columnas fuera de rango, debe ser &gt;= 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el número de columnas de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede seleccionar texto en un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede borrar texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el color de relleno del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer el color de trazo del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede establecer la saturación del texto de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Sólo se pueden enlazar marcos de texto.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>El marco de destino debe estar vacío.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>El marco de destino enlaza con otro marco.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>El marco de destino está enlazado por otro marco.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>El origen y el destino son el mismo objeto.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>No se puede desenlazar un marco que no es de texto.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation type="obsolete">El objeto no es un marco de texto enlazado, no se puede desenlazar.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="obsolete">El objeto es el último marco en una serie, no se puede desenlazar. Desenlaza el marco anterior.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>No se puede convertir un marco que no es de texto a contornos.</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian)</source>
        <translation type="obsolete">Portugués (Brasileño)</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation type="obsolete">No se puede tener un color con un nombre vacío.</translation>
    </message>
    <message>
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
        <translation type="obsolete">No se pudo guardar el pixmap.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede poner un marcador en marco que no es de texto.</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>No se puede obtener información de un marco que no es de texto.</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Documentos de Texto OpenDocument</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Croata</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Portugués</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished">Portugués (BR)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Scribus se ha cerrado</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Scribus se ha cerrado debido a la señal #%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Personalizado</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Página</translation>
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
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"> p</translation>
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
        <translation type="unfinished">Puntos (pt)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Pulgadas (in)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Picas (p)</translation>
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
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Reemplazar</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished">Todos</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">Previsualizar &amp;Fuentes...</translation>
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
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Ejecutar Script...</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Mostrar &amp;Consola</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished">&amp;Acerca de un Script...</translation>
    </message>
    <message>
        <source>&amp;Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">&amp;Scripts de Scribus</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">Scripts &amp;Recientes</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished">Acerca del Script</translation>
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
        <source>Copy #%1 of </source>
        <translation type="unfinished">Copia #%1 de </translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Negro</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Cian</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Magenta</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Amarillo</translation>
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
        <translation type="unfinished">Nuevo desde Plantilla</translation>
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
        <translation type="unfinished">Palabras Cortas</translation>
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
    <message>
        <source>Scribus crashes due to the following exception : %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Creating Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Modified Font found, checking...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Writing updated Font Cache</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Buscando fuentes</translation>
    </message>
    <message>
        <source>You are running a development version of Scribus 1.3.x. The document you are working with was created in Scribus 1.2.3 or lower. The process of saving will make this file unusable again in Scribus 1.2.3 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The changes to your document have not been saved and you have requested to revert them. Do you wish to continue?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A file named &apos;%1&apos; already exists.&lt;br/&gt;Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>firstPageOrder is bigger than allowed.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Old .sla format support</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>German (Trad.)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Exporting PostScript File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printing File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <source>Thai</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Barcode Generator</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd *.odg);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Word Documents</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Palm PDB Documents</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB_data</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PDB Import</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open file %1</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This file is not recognized as a PDB document propably. Please, report this as a bug if you are sure it is one.</source>
        <comment>PDB Importer</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>Re&amp;hacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menú del Sistema</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Desenrollar</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Normalizar</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Maximizar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurar</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Mover</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamaño</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Mi&amp;nimizar</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ma&amp;ximizar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Mantener sobre el resto</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Restaurar Tamaño</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Cerrar</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>Rec&amp;oger</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Desenrollar</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Document Setup</source>
        <translation>Configuración del documento</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Tamaño:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Orientación:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Anchura:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Altura:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guías de los márgenes</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Izquierda:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>D&amp;erecha:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">Páginas en&amp;frentadas</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Primero página izquierda</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation type="obsolete">Núme&amp;ro de la primera página:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Esquema de página única o doble</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Hacer la primera página del documento la página izquierda</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen superior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Distancia entre la guía del margen inferior y el borde de la página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen izquierdo y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Distancia entre la guía del margen derecho y el borde de la página.
Si las páginas enfrentadas están activadas, este espacio se puede usar
para lograr los márgenes correctos para la encuadernación.</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Interior:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">&amp;Exterior:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Orientación:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">Anc&amp;hura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">Al&amp;tura:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="obsolete">Opciones</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Autoguardado</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">min</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">Interva&amp;lo:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Documento</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Información del documento</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Guías</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Color:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Mostrar el Área &amp;No Imprimible con el Color del Margen</translation>
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
        <translation type="unfinished">Visualización</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Tipografía</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="unfinished">Herramientas</translation>
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
        <translation type="unfinished">Color del papel</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Enmascarar el área fuera de los margenes con el color del margen</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Ajustando Colores</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply size settings to all pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Sections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply the page size changes to all existing pages in the document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Scripts de Python (*.py);; Todos los archivos (*)</translation>
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
        <source>Style Settings</source>
        <translation>Configuración de Estilo</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo del párrafo</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>Fill Color Settings</source>
        <translation>Configuración del Color de Relleno</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Color de relleno del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturación del color de relleno</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Configuración del Color de Trazo</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Color del trazo del texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturación del color de trazo</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source>Font Settings</source>
        <translation>Configuración de Fuente</translation>
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
        <translation>Fuente del texto seleccionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Escalar la anchura de los caracteres</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Character Settings</source>
        <translation>Configuración de Carácter</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation type="obsolete">Espacio entre caracteres:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Espacio entre caracteres manual</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished">Guardar la página como &amp;SVG...</translation>
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
        <translation type="unfinished">Importar &amp;SVG...</translation>
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
    <message>
        <source>Scalable Vector Graphics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG file contains some unsupported features</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SWDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Palabras Cortas</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Aplicar un espacio inseparable en:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Marcos Seleccionados</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Página Activa</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Todos los objetos</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Sólo se procesan los marcos seleccionados</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Sólo se procesa la página actual</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">Se procesan todos los objetos del documento.</translation>
    </message>
</context>
<context>
    <name>SWPrefsGui</name>
    <message>
        <source>User settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;Reset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save user configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reload system wide configuration and remove user defined one</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit custom configuration. If you save it, it will be used over system wide configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Palabras Cortas</translation>
    </message>
    <message>
        <source>User configuration exists elready. Do you really want to overwrite it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write file %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>User settings saved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>System wide configuration reloaded</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot open file %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished">Guardar como P&amp;lantilla...</translation>
    </message>
    <message>
        <source>Save a document as a template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save a document as a template. Good way to ease the initial work for documents with a constant look</source>
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
        <translation type="obsolete">Importar</translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished">Acción</translation>
    </message>
    <message>
        <source>Load/Save/Import/Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Desconocido</translation>
    </message>
</context>
<context>
    <name>ScToolBar</name>
    <message>
        <source>Top</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right</source>
        <translation type="unfinished">Derecha</translation>
    </message>
    <message>
        <source>Bottom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left</source>
        <translation type="unfinished">Izquierda</translation>
    </message>
    <message>
        <source>Allow Docking To...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertical</translation>
    </message>
    <message>
        <source>Floating Orientation...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScWinPrint</name>
    <message>
        <source>Printing...</source>
        <translation type="unfinished">Imprimiendo...</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copia #%1 de </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Fondo</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation type="obsolete">Archivo</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Crear un nuevo documento</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Abrir un documento</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Guardar el documento actual</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Cerrar el documento actual</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Imprimir el documento actual</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Guardar el documento actual como PDF</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="obsolete">Buscando fuentes</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Saliendo</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation type="obsolete">Error fatal</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="obsolete">Sistema de Fuentes Iniciado</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">División Silábica Inteligente</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Alinear a la izquierda</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Alinear a la derecha</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Centrar</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Insertar Número de Página</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Adjuntar Texto al Trazado</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Mostrar Capas</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">JavaScripts...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Deshacer</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Mostrar Paleta de Páginas</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Bloquear/Desbloquear</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Espacio no separable</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="obsolete">Leyendo las Preferencias</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Obteniendo los Perfiles ICC</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Iniciar el Divisor Silábico</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="obsolete">Configurando los Accesos Rápidos</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">Ajustes de &amp;Color...</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation type="obsolete">Leyendo el Álbum de recortes</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="obsolete">Iniciando las extensiones</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Abrir...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="obsolete">Abrir &amp;reciente</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Nuevo</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Abrir...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Cerrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Guardar</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Guardar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Gu&amp;ardar como...</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Guardar como...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Revertir al guardado</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Rec&amp;olectar para salida...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">&amp;Cargar texto/imagen...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">&amp;Añadir texto...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Importar &amp;página(s)...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">&amp;Importar</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Guardar el &amp;texto...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Guardar la página como &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Guardar como P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="obsolete">&amp;Exportar</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">In&amp;formación del documento...</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Información del documento...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Co&amp;nfiguración del documento...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Configuración del documento...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Imprimir...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Imprimir...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Salir</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Salir</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Deshacer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">B&amp;orrar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Seleccionar &amp;todo</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Cortar</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Copiar</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Pegar</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Borrar</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Seleccionar todo</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Buscar/Reemplazar...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">C&amp;olores...</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Colores...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">Estilos de &amp;párrafo...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Estilos de &amp;línea...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Estilos...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Plantillas...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Plantillas...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">&amp;JavaScripts...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Seleccionar nueva fuente</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Duplicar</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Duplicar</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">Duplicación &amp;múltiple</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Duplicación múltiple</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Borrar</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Borrar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">Agr&amp;upar</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Agrupar</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Desagrupar</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Desagrupar</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Blo&amp;quear</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">&amp;Enviar al fondo</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Enviar al fondo</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">&amp;Traer al frente</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Traer al frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Bajar</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Bajar</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Subir</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Subir</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Distribuir/&amp;Alinear...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Distribuir/Alinear...</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Editar forma...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="obsolete">&amp;Forma</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">&amp;Adjuntar Texto al Trazado</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">&amp;Desadjuntar Texto del Trazado</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">&amp;Combinar Polígonos</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Dividir &amp;Polígonos</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">C&amp;onvertir a Contorno</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Insertar...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Insertar...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Borrar...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Borrar...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Mover...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Mover...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Aplicar Plantilla...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Aplicar Plantilla ...</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Administrar &amp;Guías...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Administrar Guías...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">&amp;Ajustar a la ventana</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Ajustar a la ventana</translation>
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
        <translation type="obsolete">&amp;Miniaturas</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Miniaturas</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Mostrar Má&amp;rgenes</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Ocultar Márgenes</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Mostrar &amp;Marcos</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Ocultar Marcos</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">Mostrar &amp;Imágenes</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Ocultar Imágenes</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Mostrar &amp;Cuadrícula</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Mostrar Cuadrícula</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Mostrar &amp;Guías</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Mostrar Líneas de &amp;Base</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">&amp;Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Ajustar a la Cuadrícula</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">A&amp;justar a las Guías</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Propiedades</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Propiedades</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">&amp;Esquema</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Esquema</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">Álb&amp;um de recortes</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Álbum de recortes</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">&amp;Capas</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">Ocultar Paleta de P&amp;ágina</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Marcadores</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Ad&amp;ministrar imágenes</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Administrar imágenes</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">&amp;Dividir en sílabas</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Dividir en sílabas</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Ayuda &amp;emergente</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation type="obsolete">&amp;Herramientas</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">Herramientas de P&amp;DF</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Herramientas</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Ayuda emergente</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">&amp;Preferencias...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Fuentes...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Fuentes...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">&amp;División silábica...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">&amp;Accesos rápidos...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Acerca de Scribus</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Acerca de Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Acerca de &amp;Qt</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation type="obsolete">Acerca de Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">&amp;Manual de Scribus</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Ayuda en línea...</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="obsolete">&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Editar</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="obsolete">E&amp;stilo</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="obsolete">&amp;Objeto</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation type="obsolete">&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="obsolete">&amp;Ver</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="obsolete">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">&amp;Preferencias</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="obsolete">&amp;Ventanas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">A&amp;yuda</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Izquierda</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">&amp;Centro</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Derecha</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Justificado</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Forzado</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Otro...</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Subrayado</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Tachar</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Versalitas</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Superíndice</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Subíndice</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Contorneado</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="obsolete">Posición X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="obsolete">Posición Y:</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="obsolete">Preparado</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Cascada</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Mosaico</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Cargar texto/imagen...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Cargar imagen...</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="obsolete">&amp;Color</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Invertir</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Car&amp;gar texto...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="obsolete">&amp;Fuente</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="obsolete">&amp;Tamaño</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="obsolete">&amp;Efectos</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="obsolete">&amp;Alineación</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="obsolete">&amp;Saturación</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">Ta&amp;buladores...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Cargar texto...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Fuente</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="obsolete">Tamaño</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Estilo</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Color</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="obsolete">Saturación</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">Desblo&amp;quear</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="obsolete">Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="obsolete">Importar páginas...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="obsolete">Importar Página(s)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation type="obsolete">&lt;p&gt;Estás intentando importar más páginas que las disponibles en este documento a patir de la página actual.&lt;/p&gt;Elige una de las siguientes opciones:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Crear&lt;/b&gt; las páginas que falten&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importar&lt;/b&gt; las páginas hasta la últma&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancelar&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="obsolete">Crear</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Importar</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="obsolete">Importación realizada</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="obsolete">No hay nada que importar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">El archivo %1 no está en formato de Scribus</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="obsolete">Cargando...</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="obsolete">Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="obsolete">Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">No puedo escribir el archivo: 
%1</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="obsolete">Guardar como</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="obsolete">Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="obsolete">Guardando...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="obsolete">Imprimiendo...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Documento</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="obsolete">La impresión falló</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="obsolete">Manual de Scribus</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="obsolete">Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="obsolete">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="obsolete">&amp;Saturación:</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Ocultar Líneas de Base</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Mostrar Líneas de Base</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Faltan los siguientes programas:</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript : No puedes usar imágenes EPS</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Todos</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">Archivos EPS (*.eps);;Todos los archivos (*)</translation>
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
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Algunos objetos están bloqueados.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Cancelar</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Bloquear todo</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Desbloquear todo</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Desbloquear</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Bloquear</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Cargando:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Ajustando Colores</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Deshacer borrar objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">&amp;Deshacer mover objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">&amp;Deshacer cambios en el objeto</translation>
    </message>
    <message>
        <source>German</source>
        <translation type="obsolete">Alemán</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation type="obsolete">Polaco</translation>
    </message>
    <message>
        <source>English</source>
        <translation type="obsolete">Inglés</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation type="obsolete">Español</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation type="obsolete">Italiano</translation>
    </message>
    <message>
        <source>French</source>
        <translation type="obsolete">Francés</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation type="obsolete">Ruso</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation type="obsolete">Danés</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation type="obsolete">Eslovaco</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation type="obsolete">Húngaro</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation type="obsolete">Checo</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation type="obsolete">Holandés</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="obsolete">Portugués</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation type="obsolete">Ucraniano</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation type="obsolete">Griego</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation type="obsolete">Catalán</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation type="obsolete">Finlandés</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation type="obsolete">Irlandés</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation type="obsolete">Lituano</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation type="obsolete">Sueco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation type="obsolete">Esloveno</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="obsolete">Afrikaans</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Elige un directorio</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Invertir</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Scribus se ha cerrado</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Scribus se ha cerrado debido a la señal #%1</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="obsolete">Portugués (BR)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation type="obsolete">No hay fuentes adecuadas en tu sistema</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;La administración de color está soportada pero no está activada actualmente. Asegurate que tienes perfiles de color ICC instalados y que la ruta de los perfiles en las preferencias apunta a donde están instalados.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;La administración de color no está soportada en esta compilación de Scribus.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation type="obsolete">Croata</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Editable</translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="obsolete">Nivel</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">E&amp;nviar a Capa</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">Opciones de &amp;PDF</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="obsolete">Carácter</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Postscript</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="obsolete">Guardar como</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation type="obsolete">Bulgaro</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Información</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">no se encuentra</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Nombre:</translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="obsolete">Ejemplo</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Color</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
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
        <translation type="unfinished">Nueva Capa</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Fondo</translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusMainWindow</name>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation type="unfinished">Iniciando las extensiones</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation type="unfinished">Leyendo las Preferencias</translation>
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
        <source>Reading Scrapbook</source>
        <translation type="unfinished">Leyendo el Álbum de recortes</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation type="unfinished">Configurando los Accesos Rápidos</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="unfinished">Archivo</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished">Editable</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation type="unfinished">Buscando fuentes</translation>
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
        <source>Fatal Error</source>
        <translation type="unfinished">Error fatal</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation type="unfinished">Sistema de Fuentes Iniciado</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Archivo</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation type="unfinished">Abrir &amp;reciente</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation type="unfinished">&amp;Exportar</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished">&amp;Editar</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation type="unfinished">E&amp;stilo</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation type="unfinished">&amp;Color</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation type="unfinished">&amp;Tamaño</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation type="unfinished">&amp;Saturación</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation type="unfinished">&amp;Fuente</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation type="unfinished">&amp;Efectos</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation type="unfinished">&amp;Objeto</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">Nivel</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">E&amp;nviar a Capa</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">Opciones de &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation type="unfinished">&amp;Forma</translation>
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
        <translation type="unfinished">Carácter</translation>
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
        <source>&amp;Page</source>
        <translation type="unfinished">&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation type="unfinished">&amp;Ver</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation type="unfinished">&amp;Ventanas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">A&amp;yuda</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation type="unfinished">&amp;Alineación</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation type="unfinished">Preparado</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Abrir</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation type="unfinished">Importar páginas...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation type="unfinished">Importar Página(s)</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation type="unfinished">Importación realizada</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation type="unfinished">No hay nada que importar</translation>
    </message>
    <message>
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation type="unfinished">Cargando...</translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation type="unfinished">Todos los formatos soportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation type="unfinished">Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation type="unfinished">Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Guardar como</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation type="unfinished">Guardando...</translation>
    </message>
    <message>
        <source>Scribus has detected some errors. Consider using the Preflight Verifier to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation type="unfinished">Imprimiendo...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Documento</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation type="unfinished">La impresión falló</translation>
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
        <source>About Qt</source>
        <translation type="unfinished">Acerca de Qt</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation type="unfinished">Manual de Scribus</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation type="unfinished">Guardar como</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation type="unfinished">Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished">Normal</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Nombre:</translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Tamaño:</translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">Tamaño</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation type="unfinished">&amp;Saturación:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation type="unfinished">Saturación</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation type="unfinished">Sin estilo</translation>
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
        <source>All</source>
        <translation type="unfinished">Todos</translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
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
        <source>&amp;Lock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Unlock All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Información</translation>
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
        <source>&amp;Tools</source>
        <translation type="unfinished">&amp;Herramientas</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation type="unfinished">Posición X:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation type="unfinished">Posición Y:</translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>New Master Page %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Number of copies: %1
Horizontal shift: %2
Vertical shift: %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or PostScript Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript is missing : Postscript Print Preview is not available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to replace your existing image?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusQApp</name>
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
        <source>Uses xx as shortcut for a language, eg `en&apos; or `de&apos;</source>
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
        <source>Use right to left dialog button ordering (eg. Cancel/No/Yes instead of Yes/No/Cancel)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>filename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use filename as path for user given preferences</source>
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
        <source>Scribus Version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation type="unfinished">Página inicial</translation>
    </message>
    <message>
        <source>Documentation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Issues</source>
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
        <translation>Capa</translation>
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
        <source>All</source>
        <translation type="obsolete">Todos</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Copia de </translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Copiar aquí</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Mover aquí</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Pegar</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="unfinished">Imagen</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Archivo: </translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">PPP originales: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">PPP actuales: </translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Texto enlazado</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Marco de Texto</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Texto sobre trazado</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Párrafos: </translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Caracteres: </translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Imprimir: </translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Activado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Desactivado</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">In&amp;formación</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">Opciones de &amp;PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">E&amp;nviar a Capa</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">Nive&amp;l</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">Con&amp;vertir a</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">¿Realmente desea borrar todo el texto?</translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
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
    <message>
        <source>Colorspace: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">Desconocido</translation>
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
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation type="obsolete">&amp;Dejar como está</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation type="obsolete">C&amp;errar sin guardar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Documento:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>El documento se ha modificado desde la última vez que lo guardó.</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation type="obsolete">&amp;Guardar ahora</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Discard</source>
        <translation type="unfinished"></translation>
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
        <translation type="obsolete">&amp;Scripts de Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Ejecutar Script...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Ejecutar un script de Python desde un archivo.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="obsolete">Cargar Script de E&amp;xtensión...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Cargar un script de Python como una extensión. Usado para cargar macros y para scripts de Python avanzados que extienden la interfaz de usuario de Scribus.</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">Scripts &amp;Recientes</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Mostrar &amp;Consola</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Muestra una consola de Python interactiva donde puedes escribir y ejecutar programas de Python que usan las herramientas del Scripter.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;Acerca de un Script...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">S&amp;cript</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="obsolete">&amp;Opciones de Scripter</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Abrir</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Scripts de Python (*.py);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation>Error en el Script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Si estás ejecutando un script oficial, por favor informalo en &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Este mensaje está en tu portapapeles también. Usa Ctrl+V para pegarlo en el seguimiento de errores</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation type="obsolete">Scribus - Extensión de Script</translation>
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
        <translation type="obsolete">La función &apos;Cargar Script&apos; de la extensión de script actualmente está desactivada. 
Si quieres ejecutar un script normal, probablemente quieras usar &apos;Ejecutar Script...&apos; 
en vez de esto. 

Si realmente quieres cargar una extensión de Python o una macro, tienes que ir a 
las Opciones de Scripter en el menú Script y activar las extensiones de scripter allí.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Ocultar &amp;Consola</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">Acerca del Script</translation>
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
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Python Scripts (*.py);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> doesn&apos;t contain any docstring!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation type="obsolete">Scribus - Preferencias de Scripter</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="obsolete">Activar Extensiones de Scripter</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="obsolete">Activar scripts de extensión y macros</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Activar las extensiones de scripter activa varias funcionalidades del scripter adicionales, incluyendo macros de Python y la opción de cargar un script de Python al inicio. Activar está opción desbloquea &lt;tt&gt;Cargar Script de Extensión&lt;/tt&gt; en el menú Script.&lt;/p&gt;
&lt;p&gt; 
Sólo los scripts escritos para ser ejecutados como scripts de extensión se deberían usar con &lt;tt&gt;Cargar Script de Extensión&lt;/tt&gt; o como scripts de inicio. Ver la documentación de scripter para más detalles.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="obsolete">Usar un Script de Inicio</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Si &lt;tt&gt;Usar un Script de Inicio&lt;/tt&gt; está activado, Scribus cargará al inicio el archivo especificado aquí como un script de extensión. Es importante que el script esté escrito como un script de extensión, ya que si no está escrito cuidadosamente puede causar problemas.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Usar un Script de Inicio&lt;/tt&gt; se desactivará si las extensiones de scripter se desactivan, ya que los scripts de extensión no se pueden cargar sin tener activadas las extensiones de scripter.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="obsolete">Explorar...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="obsolete">Explorar para un archivo de script</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Explorar para un archivo de script&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="obsolete">Script para cargar al inicio</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;El archivo que contiene el script de Python que se ejecutará al inicio como un script de extensión.&lt;/p&gt;
&lt;p&gt;Nota que cuando este script se ejecute, Scribus no se ha iniciado completamente y el espacio de trabajo todavía no existe.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="obsolete">Archivo de Script:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="obsolete">Cerrar sin guardar los cambios</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="obsolete">Guardar los cambios y cerrar</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Opciones Avanzadas</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="obsolete">Importar todos los nombres al inicio</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="obsolete">Ejecutar &apos;from scribus import *&apos; en la consola de script al inicio</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Importar todos los nombres al inicio&lt;/tt&gt; es una opción avanzada. Probablemente deberías dejarla activada a menos que hayas leído la documentación y sepas que estás haciendo.&lt;/p&gt; 
&lt;p&gt;Desactivar esta opción evitará que el scripter ejecute su habitual comando &lt;tt&gt;from scribus import *&lt;/tt&gt; cuando se carga al inicio el intérprete principal (usado para la consola de script y los scripts de extensión).&lt;/p&gt; 
&lt;p&gt;Esta opción no tiene efecto hasta que Scribus se reinicie.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="obsolete">Activar Alias de Nombres Antiguos</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Activar el uso de nombres de función antiguos&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Activar Alias de Nombres Antiguos&lt;/tt&gt; es una opción avanzada. Probablemente deberías dejarla como está&lt;/p&gt; 
&lt;p&gt;Si está activada, esta opción hará que el scripter cree un gran número de alias de nombres de funciones y constantes para compatibilidad con scripts para la versión 1.2.0. Por defecto está activada.&lt;/p&gt; 
&lt;p&gt;Esta opción no tiene efecto hasta que Scribus se reinicie.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="obsolete">Usar Entrada Estándar Falsa</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Reemplaza sys.stdin con un archivo falso para evitar que Scribus se cuelgue cuando un script intente leer de la entrada estándar.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Usar Entrada Falsa&lt;/tt&gt; es una opción avanzada. Probablemente deberías dejarla como está.&lt;/p&gt; 
&lt;p&gt;Normalmente, scribus proporcionará a Python un objeto falso para &lt;tt&gt;sys.stdin&lt;/tt&gt;, para que al leer de la entrada estándar siempre obtenga una cadena vacía. Si se deja el real &lt;tt&gt;sys.stdin&lt;/tt&gt;, los scripts que intentan leer de ella se bloquearán - y bloquearán la ejecución de scribus, haciendo que la aplicación parezca que se cuelga - hasta que llegue algo a la entrada estándar. Es inusual que las aplicaciones gráficas esperen algo de la entrada estándar, así que la mayoría de los usuarios pensarán que scribus se ha colgado.&lt;/p&gt; 
&lt;p&gt;Puedes desactivar esta opción si quieres aceptar datos de la entrada estándar. Generalmente deberías usar &lt;tt&gt;os.popen&lt;/tt&gt; para hacer una tubería, o usar algún otro mecanismo de entrada, pero esta opción está aquí por si acaso.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Console</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Startup Script:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Errors:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Comments:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Keywords:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Signs:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Numbers:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Strings:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Base Texts:</source>
        <comment>syntax highlighting</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Change...</source>
        <translation type="unfinished">Cambiar...</translation>
    </message>
    <message>
        <source>Locate Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Mostrar Previsualización de Páginas</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Mostrar nombres de plantilla</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Buscar/Reemplazar</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Buscar:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Estilo de Párrafo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fuente</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamaño de la fuente</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efectos de Fuente</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Color de Relleno</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Saturación de Relleno</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Color de Trazo</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Saturación de Trazo</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Izquierda</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecha</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Justificado</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forzado</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Ninguno</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Reemplazar con:</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Palabras Completas</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorar mayúsculas/minúsculas</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Buscar</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Reemplazar</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Reemplazar &amp;Todo</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Búsqueda finalizada</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
    </message>
    <message>
        <source>Search finished, found %1 matches</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation type="obsolete">Ordenar Páginas</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Plantillas Disponibles:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation type="obsolete">Páginas del Documento:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation type="obsolete">Páginas enfrentadas</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Primero la página izquierda</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Arrastrar páginas o plantillas a la papelera para borrarlas.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Aquí puedes ver todas las páginas del documento.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Aquí están todas tus plantillas; para crear una nueva página
arrastra una plantilla en la vista de páginas de abajo.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Normal</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Seleccionar Campos</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Campos disponibles</translation>
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
        <translation>Campos Seleccionados</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Otro...</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Saturación:</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Saturación</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
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
        <translation type="unfinished">&amp;Palabras Cortas...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Palabras Cortas</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Sin estilo</translation>
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
        <translation>Sin estilo</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Editar Estilos</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">A&amp;ñadir</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Duplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Guardar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Nuevo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">¿Realmente desea borrar este Estilo?</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">No</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Sí</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);; Todos los archivos (*)</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Editor Interno</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Recargar Texto del Marco</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Guardar a archivo...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Cargar de archivo...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Guardar &amp;documento</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>&amp;Actualizar el Marco de Texto y Salir</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>&amp;Salir sin Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Seleccionar &amp;todo</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Cor&amp;tar</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Pegar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Buscar/Reemplazar...</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Insertar Carácter Especial...</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Editar Estilos...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Previsualizar &amp;Fuentes...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>Fondo del &amp;editor...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Fuente a usar...</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Selección de texto inteligente</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Preferencias</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Archivo</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Borrar todo el texto</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation type="obsolete">Cargar Texto de Archivo</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation type="obsolete">Guardar Texto a Archivo</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation type="obsolete">Actualizar el Marco de Texto y Salir</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation type="obsolete">Salir sin Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation type="obsolete">Recargar Texto del Marco</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation type="obsolete">Actualizar el Marco de Texto</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation type="obsolete">Buscar/Reemplazar</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Párrafo actual:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palabras: </translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres: </translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Totales:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Párrafos: </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>¿Desea guardar los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">¿Realmente desea perder todos los cambios?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">¿Realmente desea borrar todo el texto?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Archivos de texto (*.txt);;Todos los archivos (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Guardar como</translation>
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
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Insertar</translation>
    </message>
    <message>
        <source>Character</source>
        <translation type="unfinished">Carácter</translation>
    </message>
    <message>
        <source>Quote</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Spaces &amp;&amp; Breaks</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ligature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Space</source>
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
        <translation type="unfinished"> %</translation>
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
        <translation>Subrayado</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Versalitas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subíndice</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Superíndice</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Tachado</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Contorno</translation>
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
        <translation type="obsolete">Esquema</translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Outline</source>
        <comment>Text Style Selector</comment>
        <translation type="unfinished">Esquema</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Opciones de la Importación de OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Usa nombre del documento como un prefijo para los estilos de párrafo</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>No preguntar de nuevo</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Update Paragraph Styles</source>
        <translation type="obsolete">Actualizar Estilos de Párrafo</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Activar esto sobreescribirá los estilos existentes en el documento actual</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Fusionar Estilos de Párrafo</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Fusionar estilos de párrafo por atributos. Esto resultará en menos estilos de párrafo similares, retendrá los atributos de los estilos, incluso si los estilos del documento original se llamaban de forma diferente.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Anteponer el nombre del documento al estilo de párrafo de Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Establecer estas opciones por defecto y no preguntar de nuevo cuando se importe un documento de OpenOffice.org 1.x</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Cancelar</translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation type="unfinished">Ninguno</translation>
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
        <translation type="unfinished">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
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
    <name>TOCIndexPrefsBase</name>
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
        <translation type="unfinished">A&amp;ñadir...</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+T</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Borrar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+B</translation>
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
        <translation type="unfinished"> ppp</translation>
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
        <translation type="unfinished"> px</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Color:</translation>
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
        <translation type="unfinished">Mostrar Líneas de Base</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">Líneas de &amp;Base:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Mar&amp;gen de las Líneas de Base:</translation>
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
        <translation type="unfinished">Distancia entre las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Distancia entre las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Distancia dentro de la que un objeto se ajustará a las guías</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Radio del área donde Scribus permitirá que se seleccione un objeto</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Color de las líneas menores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Color de las líneas mayores de la cuadrícula</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Color de las guías que se insertan</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Color de las líneas de margen</translation>
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
        <translation>Administrar los tabuladores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Rango de exportación</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;Todas las páginas</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">&amp;Rango</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Rotación:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Opciones del Archivo</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">Compat&amp;ibilidad:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">E&amp;ncuadernación:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Margen Izquierdo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Margen Derecho</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Generar &amp;miniaturas</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Guardar marcos enla&amp;zados como artículos PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">Incluir &amp;marcadores</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished"> ppp</translation>
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
        <translation type="unfinished">Configuración de imágenes</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Ninguno</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">Mét&amp;odo:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">Cali&amp;dad:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Máxima</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Alta</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Baja</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Mínima</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">G&amp;eneral</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">&amp;Empotrar todas las fuentes</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">Empotrar &amp;subconjunto de todas las fuentes</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Empotrar</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Fuentes Disponibles:</translation>
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
        <translation type="unfinished">Fuentes a empotrar:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Fuentes a empotrar un subconjunto:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Fuentes</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Página</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">&amp;Mostrar vista previa</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Efectos</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">Duración de la vis&amp;ualización:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Duración del efect&amp;o:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">&amp;Tipo de efecto:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">Lí&amp;neas:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">&amp;Desde:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">Di&amp;rección:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished"> seg</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Sin efecto</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">Bandas</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Caja</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Disolver</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">Brillo</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">División</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">Cortina</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Horizontal</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Vertical</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Interior</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Exterior</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Izquierda a Derecha</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Arriba a Abajo</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Abajo a Arriba</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Derecha a Izquierda</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Arriba-Izda a Abajo-Dcha</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">Apl&amp;icar efectos en todas las páginas</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">Usar cif&amp;rado</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Contraseñas</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;Usuario:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">&amp;Propietario:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Configuraciones</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Permitir &amp;imprimir el documento</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Permitir ca&amp;mbiar el documento</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Permitir c&amp;opiar texto y gráficos</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Permitir a&amp;ñadir anotaciones y campos</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">&amp;Seguridad</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">General</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">Salida &amp;dirigida a:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Pantalla / Internet</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Impresora</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">&amp;Usar configuraciones de renderizado personalizadas</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Configuraciones de Renderizado</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">F&amp;recuencia:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">Á&amp;ngulo:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">Función de &amp;punto:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Punto simple</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Línea</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">Redondeado</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Elipse</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Colores Sólidos:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Usar perfil ICC</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Perfil:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished">Modo de Visualización:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished">Perceptivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Colormetraje Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Saturación</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Colormetraje Absoluto</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Imágenes:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">No usar perfiles ICC empotrados</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">Co&amp;lor</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">Modo de salida de PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">Texto &amp;informativo:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">&amp;Perfil de salida:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Cuadro de recorte</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Empotrar las fuentes en el PDF. Esto preservará
el diseño y la apariencia del documento.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Mostrar la previsualización de cada página listada arriba.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Cuanto tiempo se muestra la página antes de que la presentación empiece en la página actual.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">La duración del efecto.
Cuanto menor tiempo más rápido será el efecto, y viceversa.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Tipo del efecto que se mostrará.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Dirección del movimiento de las líneas para los efectos de división y bandas.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Posición inicial de los efectos de caja y división.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Dirección de los efectos de brillo y cortina.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Aplicar el efecto actual en todas las páginas.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Exportar todas las páginas a PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Exportar un rango de páginas a PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Inserta una lista de elementos separados por comas,
donde un elemento puede ser * para todas las páginas,
1-5 para un intervalo de páginas o un único número.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Determinar la encuadernación de las páginas en el PDF. Si no sabes
que elegir deja la opción por defecto - Izquierda.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Genera miniaturas de cada página en el PDF.
Algunos visores pueden usar las miniaturas para la navegación.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Generar artículos en el PDF, lo que es útil para navegar artículos enlazados en un PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Empotrar los marcadores que has creado en tu documento.
Son útils para navegar en documentos PDF largos.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Resolución del testo y gráficos vectoriales exportados.
Esto no afecta a la resolución de las imágenes como las fotos.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Niveles de compresión: Mínimo (25%), Bajo (50%), Medio (75%), Alto (85%), Máximo (95%)</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">PPP (puntos por pulgada) con que exportar las imágenes.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Elige una contraseña maestra que activará o desactivará todas
las características de seguridad del PDF.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Elige una contraseña para que los usuarios puedan leer el PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Permitir imprimir el PDF, si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Permitir modificar el PDF. Si no se selecciona no se permitirá.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Permitir copiar el texto o los gráficos del PDF.
Si no se selecciona no se permtirá.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Modelo de color para la salidad del PDF.
Elige Pantalla / Web para PDFs que se usarán para mostrar en pantalla e imprimir en impresoras de tinta.
Elige Impresora para imprimir en impresoras de 4 colores CMYK.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Esta es una opción avanzada que no se activa por defecto. Sólo se debería activar
cuando lo pida específicamente tu imprenta y te den los detalles exactos que necesitan.
De otra forma, puede que el PDF no se imprima apropiadamente y no será portable entre distintos sistemas.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished">Empotrar un perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Perfil de color para colores sólidos</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished">Modo de representación para colores sólidos</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Empotrar un perfil de color para las imágenes</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">No usar los perfiles de color que estén empotrados en las imágenes</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Perfil de color para imágenes</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Modos de representación para las imágenes</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Perfil de salida para impresión. Si es posible, pide consejo a tu imprenta sobre la elección del perfil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Cadena obligatoria para PDF/X-3 o el PDF no será conforme a PDF/X-3.
Recomendamos que uses el título del documento.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Distancia del margen desde el extremo superior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Distancia del margen desde el extremo inferior de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Distancia del margen desde el extremo izquierdo de la página real</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Distancia del margen desde el extremo derecho de la página real</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Voltear página(s) horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Voltear página(s) verticalmente</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression &amp;Quality:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is PDF 1.3 which gives the widest compatibility. Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption. PDF/X-3 is for exporting the PDF when you want color managed RGB for commercial printing and is selectable when you have activated color management. Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression quality levels for lossy compression methods: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%). Note that a quality level does not directly determine the size of the resulting image - both size and quality loss vary from image to image at any given quality level.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. If unchecked, text and graphics cannot be copied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. If unchecked, editing annotations and fields is prevented.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables Spot Colors to be converted to composite colors. Unless you are planning to print spot colors at a commercial printer, this is probably best left enabled.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Include La&amp;yers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression Metho&amp;d:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Resa&amp;mple Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts will preserve the layout and appearance of your document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Length of time the effect runs. A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where a token can be * for all the pages, 1-5 for a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know you need to change it leave the default choice - Left.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF. Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document. These are useful for navigating long PDF documents.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics. This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enables lossless compression of text and graphics. Unless you have a reason, leave this checked. This reduces PDF file size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI. Leaving this unchecked will render them at their native resolution. This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF. If you selected PDF 1.3, the PDF will be protected by 40 bit encryption. If you selected PDF 1.4, the PDF will be protected by 128 bit encryption. Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the security features in your exported PDF</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color model for the output of your PDF. Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets. Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled when specifically requested by your printer and they have given you the exact details needed. Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page Layout</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Continuous</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double Page Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Visual Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Viewers Defaults</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Full Screen Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Bookmarks Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Thumbnails</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display Layers Tab</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hide Viewers Menubar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Zoom Pages to fit Viewer Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Special Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Javascript to be executed
when Document is opened:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Viewer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Clip to Page Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossy - JPEG</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Lossless - Zip</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Fuente:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"> pt</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Tamaño:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Ninguno</translation>
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
        <translation type="unfinished">Colu&amp;mnas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">Es&amp;pacio:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Woven silk pyjamas exchanged for blue quartz</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">Color de &amp;línea:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Saturación:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">Color de &amp;Relleno:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">Sat&amp;uración:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Escalado Li&amp;bre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">Escalado &amp;Horizontal</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">Escalado &amp;Vertical</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">&amp;Escalar Imagen al Tamaño del Marco</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">&amp;Mantener la Relación de Aspecto</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">Color de &amp;relleno:</translation>
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
        <translation type="unfinished">Mí&amp;nimo</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">Má&amp;ximo</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Incremento:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Propiedades de Marco de Texto</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Propiedades de Marco de Imagen</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Propiedades de Dibujo de Formas</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Niveles de Zoom por Defecto</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Propiedades de Dibujo de Líneas</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Propiedades de Dibujo de Polígonos</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished">Tamaño de la fuente de los marcos de texto nuevos</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Color de la fuente</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Número de columnas en un marco de texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Hueco entre las columnas del marco de texto</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Ejemplo de la fuente</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Los marcos de imagen permiten el escalado de la imagen a cualquier tamaño</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Escalado horizontal de las imágenes</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Escalado vertical de las imágenes</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished">Mantener iguales los escalados horizontal y vertical</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Las imágenes se escalan al tamaño del marco</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Mantener las proporciones originales de las imágenes escaladas automáticamente</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Color de relleno de los marcos de imagen</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Saturación del color de relleno</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Color de línea de las formas</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Saturación del color de las líneas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Color de relleno de las formas</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Estilo de línea de las formas</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Ancho de línea de las formas</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Reducción mínima permitida</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Ampliación máxima permitida</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">Cambio en el nivel de ampliación por cada operación de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Color de las líneas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Saturación del color</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Estilo de las líneas</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Ancho de las líneas</translation>
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
        <translation type="unfinished">Personalizado</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Subíndice</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished"> %</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">&amp;Desplazamiento</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">&amp;Escala:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Superíndice</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Des&amp;plazamiento</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">E&amp;scala:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished">Subrayado</translation>
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
        <translation type="unfinished">Ancho de linea:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">Tachar</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Versalitas</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">Esca&amp;la:</translation>
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
        <translation type="unfinished">Desplazamiento sobre la línea de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Tamaño relativo del superíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Desplazamiento por debajo de la línea base de la fuente en una línea</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Tamaño relativo del subíndice comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Tamaño relativo de las versalitas comparado a la fuente normal</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Porcentaje de incremente sobre el tamaño de fuente para el interlineado</translation>
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
        <translation>Izquierda</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Derecha</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Punto</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Coma</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centrado</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>P&amp;osición:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Pri&amp;mera Línea:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">&amp;Sangría izquierda:</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Borrar Todos</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Sangría de la primera línea del párrafo</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Sangría izquierda de todo el párrafo</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Borrar todos los tabuladores</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete"> pt</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete"> mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete"> in</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete"> p</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished">Ninguno</translation>
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
        <translation type="unfinished">Personalizado</translation>
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
        <translation>Esquema</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Tipo</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Información</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Atención</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">El nombre &quot;%1&quot; no es único.
Por favor elige otro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
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
        <translation type="obsolete">Fuente:</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grupo </translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Imagen</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Línea</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Polilínea</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">TextoTrazado</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Página</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Página </translation>
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
        <translation type="unfinished"> %</translation>
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
        <translation type="unfinished">Agrupar</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Crear</translation>
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
        <translation type="unfinished">Cancelar</translation>
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
        <translation type="unfinished">Bloquear</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Desbloquear</translation>
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
        <translation type="unfinished">Borrar</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Renombrar</translation>
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
        <translation type="unfinished">Pegar</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Cortar</translation>
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
        <translation type="unfinished">Polígono</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Polilínea</translation>
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
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Texto sobre trazado</translation>
    </message>
    <message>
        <source>Enable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Disable Item Printing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Multiple duplicate</source>
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
        <translation type="unfinished">&amp;Deshacer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">Re&amp;hacer</translation>
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
    <name>UsePrinterMarginsDialog</name>
    <message>
        <source>Minimum Margins for Page Size %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>UsePrinterMarginsDialogBase</name>
    <message>
        <source>Use Printer Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Select &amp;Printer:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Margins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Right:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">I&amp;nferior:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Insertar valor</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Introducir un valor y presionar Aceptar.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Introducir un valor y presionar Aceptar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Enviar el valor al script</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Palabras Cortas</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Aplicar un espacio inseparable en:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Marcos Seleccionados</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Página Activa</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Todos los objetos</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Información e Idiomas</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Reemplazar valores por defecto por configuración de usuario</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Cuando el archivo de configuración de usuario existe 
(%1)
puedes elegir si quieres añadir tu configuración a la 
global desactivando está opción.

También se pueden reemplazar los valores predefinidos 
por los propios con la opción activada.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Sólo se procesan los marcos seleccionados</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Sólo se procesa la página actual</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Se procesan todos los objetos del documento.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Palabras Cortas para Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Disponible en los siguientes idiomas</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Acerca de Palabras Cortas</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="obsolete">&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="obsolete">A&amp;yuda</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="obsolete">Palabras Cortas</translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Herramientas</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Seleccionar objetos</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Insertar marco de texto</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Insertar imagen</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Insertar tabla</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propiedades...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Insertar polígonos</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Insertar lineas</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Insertar Curva de Bézier</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Insertar linea a mano alzada</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Rotar objeto</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Acercar o alejar</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Editar contenidos del marco</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Editar el texto con el editor interno</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Enlazar marcos de texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Desenlazar marcos de texto</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Medir</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Dibujar varias formas</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>PDF Tools</source>
        <translation>Herramientas de PDF</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botón</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Casilla de selección</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Cuadro combinado</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Lista</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Insertar campos de PDF</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Enlace</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Insertar anotaciones de PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Seleccionar Carácter:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Insertar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Cerrar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Inserta los caracteres en el texto en la posición del cursor</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Borra la selección actual</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Cierra este diálogo y vuelve a la edición de texto</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige que importación usar</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automática</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importar texto sin ningún formato</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importación:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Importar sólo texto</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Elige que importación usar</translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Recordar asociación</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Recordar la asociación extensión - importación y no preguntar
de nuevo que importación hacer para archivos de este tipo.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Aceptar</translation>
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
        <translation>Nuevo desde Plantilla</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todos</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Creado con</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Fecha</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Descargando Plantillas</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Se pueden encontrar plantillas de documento en http://www.scribus.net/ en la sección descargas (Downloads).</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Instalando Plantillas</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extrae el paquete al directorio de plantillas ~/.scribus/templates para el usuario actual o en PREFIX/usr/share/scribus/templates para todos los usuarios del sistema.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Preparando una pantilla</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Asegurate que las imágenes y fuentes que usas se pueden usar libremente. Si las fuentes no se pueden compartir no las recolectes cuando lo guardes como una plantilla.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>El creador de la plantilla debería asegurarse también de que la sección de Instalar Plantillas de arriba se aplica a sus plantillas también. Esto quiere decir que un usuario debería poder descargar la plantilla y extraerla al directorio de plantillas y usarla.</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Eliminando una plantilla</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Eliminar una plantilla del diálogo Nueva desde Plantilla sólo eliminará la entrada de template.xml, no borrará los archivos. Sólo se muestra un menú contextual con la opción de eliminar si tiene permiso de escritura en el archivo template.xml</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Traduciendo template.xml</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copia un archivo template.xml que ya exista a un archivo llamado template.idioma_PAÍS.xml (usa el mismo código de idioma que en el archivo qm de tu idioma), por ejemplo template.fi.xml para Finlandés. La copia debe estar en el mismo directorio que el original template.xml para que Scribus pueda cargarlo.</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Guardar como Plantilla</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categoría</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Formato de página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descripción:</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Correo electrónico</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Más detalles</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Aceptar</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Menos detalles</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tabloide</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Apaisado</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>Personalizado</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Crear filtro</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>B&amp;orrar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Elegir un filtro guardado previamente</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Dar un nombre a este filtro para guardarlo</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Darle un nombre para guardarlo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Aceptar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Desactivar o activar esta fila del filtro</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Eliminar esta fila del filtro</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Añadir una nueva fila de filtro</translation>
    </message>
    <message>
        <source>to</source>
        <translation>a</translation>
    </message>
    <message>
        <source>and</source>
        <translation>y</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>eliminar las coincidencias</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>no eliminar las coincidencias</translation>
    </message>
    <message>
        <source>words</source>
        <translation>palabras</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Borrar</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Reemplazar</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>El valor de la izquierda es una expresión regular</translation>
    </message>
    <message>
        <source>with</source>
        <translation>con</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>estilo de párrafo</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>todas las instacias de</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>todos los párrafos</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>los párrafos que empiezan con</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>los párrafos con menos de</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>los párrafos con más de</translation>
    </message>
</context>
</TS>
