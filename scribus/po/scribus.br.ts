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
        <translation>getFontSize([&quot;name&quot;]) -&gt; float

Indíca o tamanho da fonte em polegadas no quadro de texto &quot;name&quot;. Se este texto estiver selecionado, a indicação de tamanho será relativa ao primeiro caractere. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Exibe uma lista contendo todas as cores definidas no documento. 
Se não houver nenhum documento aberto será exibida uma lista de cores padrão do documento. 

</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool 

Exibe a janela de &quot;Novo Documento&quot;. Cria um novo documento se o usuário aceitar as configurações. Não cria um documento se o usuário clicar em cancelar. 
Retorna verdadeiro se um novo documento for criado. 


</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string 

Retorna o nome da cor de preenchimento do objeto &quot;name&quot;. 
Se o \&quot;name\&quot;não for dado, a seleção atual será usada. 
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

Move o objeto &quot;name&quot; pelo eixo X e Y relativo a sua posição atual. As distâncias são expressas nos valores utilizados pelo documento (veja o valor especificado em unidades). Se o &quot;name&quot; não for dado, a seleção atual será usada. 
Se o objeto &quot;name&quot; pertencer a um grupo, todo o grupo será movido. 
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool) 

Desabilita o redesenho quando bool é igual a Falso, se não o redesenho é habilitado. 
Esta mudança persistirá mesmo depois de o script ser finalizado então, tenha certeza de chamar a função setRedraw(Verdadeiro) no nível superior de seu script. 
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

Cria um novo retângulo no documento atual e lhe dá um nome, As coordenadas são expressas nos valores utilizados pelo documento (veja o valor especificado em unidades). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para referênciar o objeto no futuro. Se o &quot;name&quot; não for dado, o Scribus, criará um para você. 

A janela NameExistError poderá ser aberta caso você escolha um nome já em uso. 
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
        <translation>newPage(where [,&quot;template&quot;]) 

Cria um nova página se &quot;where&quot; for -1 a nova página será anexada ao documento, caso contrário a nova página será insirida antes de &quot;where&quot;. o número de páginas são contados a partir do 1 para cima, não importanto o número mostrado na primeira página de seu documento. O parametro &quot;template&quot; especifica o nome da página modelo para uma nova página. 

A janela IndexError poderá ser aberta caso o número dado estiver fora de ordem. 
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;]) 

Especifica o preenchimento gradiente do objeto &quot;name&quot;. As descrições das cores são as mesma para setFillColor() e para setFillShade() Veja os valores possíveis . </translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Descreve a &quot;string&quot; na barra de mensagens do Scribus (barra de status). O texto deve estar encodificado em UTF8 ou unicode (recomendado). 
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
        <translation>newDoc(size, margins, orientation, firstPageNumber,
                   unit, facingPages, firstSideLeft) -&gt; bool 

Cria um novo documento e retorna verdadeiro caso bem sucedido. Os parametros devem seguir os seguintes significados: 

....size = As medidas (largura e altura) descrevem o tamanho do documento. Você pode usar valores predefinidos como e.g PAPER_A4 etc. 

....margins = As indicações (esquerda, direita topo e fundo) descrevem as margens do documento 

....orientation = a orientação de página é dada pelos valores RETRATO e  PAISAGEM 

....firstPageNumber = É o número da primeira página no documento usado para paginação. Enquanto você normalmente gostaria de usuar o número 1, isso é interessante caso você esteja criando um documento em várias partes. 

....unit: esses valores indicam a unidade de medida utilizada pelo documento. Use um valor predefinido para isso, como: UNIDADE_POLEGADAS, UNIDADE_MILIMITROS, UNIDADE_PICAS, UNIDADES_PONTOS. 

....pacingPages = PÁGINAS DE ROSTO, SEM PÁGINAS DE ROSTO 

....firstSideLeft = PRIMEIRA PÁGINA À DIREITA, PRIMEIRA PÁGINA À ESQUERDA 

Os valores para largura e altura e margens são expressos na unidade selecionada no documento. PAPER_* os valores são expressos em pontos. Se o seu documento não está em pontos, lembre-se de contar por isso. 

exemplo: newDOC(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS, FACINGPAGES, FIRSTPAGERIGHT) 
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>@default</name>
    <message>
        <source>getFont([&quot;name&quot;]) -&gt; string

Returns the font name for the text frame &quot;name&quot;. If this text frame
has some text selected the value assigned to the first character
of the selection is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getFont([&quot;name&quot;]) -&gt; string 

Exibe o nome da fonte para o quadro de texto &quot;name&quot;. Se esse quadro de texto conter algum texto selecionado, o valor relacionado ao primeiro caractere da seleção será exibido. Se o &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer 

Retorna o comprimento do texto do quadro de texto &quot;name&quot;. 
Se um &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;name&quot;]) -&gt; string 

Exibe o texto do quadro de texto &quot;name&quot;. Se esse quadro de texto conter algum texto selecionado, a seleção será exibida. Todo o texto no quadro, visível ou não, será exibido. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
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

Exibe o texto dos quadros de texto &quot;name&quot; e todos os quadros de texo ligados ao seu quadro. Se esse quadro de texto conter algum texto selecionado, a seleção será exibida. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float 

Exibe o entrelinhas do quadro de texto expresso em pontos. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float 

Exibe o tamanho da lacuna do quadro de texto &quot;name&quot; expresso em pontos. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer 

Conta o número de colunas do quadro de texto &quot;name&quot;. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
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

Define o texto do quadro de texto &quot;name&quot; para o texto da fila &quot;text&quot;. 
O texto deve estar encodificado em UTF8, Veja mais detalhes no FAQ . 
Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 
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
        <translation>insertText(&quot;text&quot;, pos, [&quot;name&quot;]) 

insere o texto &quot;text&quot; na posição após o quadro de texto. O texto deve estar codificado em UTF (veja setText() como referência). O primeiro caractere tem o índice de valor 0. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 

A janela IndexError poderá aparecer por uma inserção de valores fora do limite. 


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

Define a fonte do quadro de texto &quot;name&quot; para &quot;font&quot;. Se houver algum texto selecionado, apenas a seleção será afetada. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 

A janela ValueError poderá aparecer caso a fonte não for encontrada. 

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

Define o tamanho da fonte do quadro de texto &quot;name&quot; para &quot;size&quot;. &quot;size&quot; é o valor me pontos. Se houver algum texto selecionado apenas a seleção será alterada. &quot;size&quot; deve estar entre o intervalo de 1 a 512. Se &quot;name&quot; não dado</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>Se &quot;name&quot; não for dado a seleção atual será usada.</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>Se &quot;name&quot; não for dado a seleção atual será usada.</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>setTextAlignment(align, [&quot;name&quot;])

Sets the text alignment of the text frame &quot;name&quot; to the specified alignment.
If &quot;name&quot; is not given the currently selected item is used. &quot;align&quot; should
be one of the ALIGN_ constants defined in this module - see dir(scribus).

May throw ValueError for an invalid alignment constant.
</source>
        <translation>setTextAlignment(align, [&quot;name&quot;])

Define o alinhamento específico do texto no quadro de texto. 
Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. Alinhar deve ter o valor de uma das constantes de alinhamento definidas nesse módulo - veja dir(scribus). 

A janela ValueError poderá aparecer se o valor da constante for inválido. 
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
        <translation type="obsolete">selectText(start, count, [&quot;name&quot;])

Conta os caracteres selecionados do texto no quadro de texto &quot;name&quot; começando do caractere inicial. A contagem começa a partir do 0. Se a &quot;contagem&quot; for zero, qualquer texto selecionado será limpo. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. 

A janela IndexError poderá aparecer caso a seleção esteja fora do texto</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;name&quot;]) 

Apaga qualquer texto no quadro de texto &quot;name&quot;. Se houver algum texto selecionado, somente a seleção será apagada. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Define a co do texto do quadro de texto &quot;name&quot; para a cor &quot;color&quot;. Se houver algum texto selecionado, apenas a seleção será alterada. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;]) 

Define a cor da liha do texto. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. </translation>
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

Define o sombreamento da cor do texto do objeto &quot;name&quot; para a sombra. Se houver algum texto selecionado, apenas a seleção será alterada. A sombra deve ser um valor íntegro entre 0 (Muito iluminado) ao 100 (Intensidade total da Cor). Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;)

Link two text frames. The frame named &quot;fromname&quot; is linked to the
frame named &quot;toname&quot;. The target frame must be an empty text frame
and must not link to or be linked from any other frames already.

May throw ScribusException if linking rules are violated.
</source>
        <translation>linkTextFrames(&quot;fromname&quot;, &quot;toname&quot;) 

Liga dois quadros de textos. O quadro &quot;fromname&quot; esta ligado ao quadro &quot;toname&quot;. O quadro alvo deve ser um quadro de texto vazio e não deve ligar ou ser ligado com outros quadros. 

A janela ScribusException poderá aparecer caso as regras de ligação forem violadas. </translation>
    </message>
    <message>
        <source>unlinkTextFrames(&quot;name&quot;)

Remove the specified (named) object from the text frame flow/linkage. If the
frame was in the middle of a chain, the previous and next frames will be
connected, eg &apos;a-&gt;b-&gt;c&apos; becomes &apos;a-&gt;c&apos; when you unlinkTextFrames(b)&apos;

May throw ScribusException if linking rules are violated.
</source>
        <translation>unlinkTextFrames(&quot;name&quot;) 

Remove o objeto especificado da ligação/caminho do quadro de texto. Se o quadro estiver no meio do elo, o anterior e o próximo quadros serão ligados. Por exemplo, a-&gt;d-&gt;c torna-se a-&gt;c quando você desliga os quadro de texto b 

A janela ScribusException poderá aparecer caso as regras de ligação forem quebradas 
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;]) 

Converte o quadro de texto &quot;name&quot;para contornos. Se o &quot;name&quot; não for dado o ítem atualmente selecionado será usado.</translation>
    </message>
    <message>
        <source>getColor(&quot;name&quot;) -&gt; tuple

Returns a tuple (C, M, Y, K) containing the four color components of the
color &quot;name&quot; from the current document. If no document is open, returns
the value of the named color from the default document colors.

May raise NotFoundError if the named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>getColor(&quot;name&quot;) -&gt; tuple 

Retorna o valor (C, M, Y, K) da composição da cor &quot;name&quot; do documento atual. Se nenhum documento estiver aberto, retorna o valor da cor nomeada das cores padrões do documento. 

A janela NotFoundError poderá aparecer caso a cor não seja encontrada. 
A janela ValueError poderá aparecer caso um nome inválido de cor seja especificado. </translation>
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

Muda a cor &quot;name&quot; para o valor especificado em CMYK. O valor da cor é determinado por quatro componentes C = Ciano, M = Magenta, Y = Amarelo, K = Preto. 
Os valores dos componentes devem estar entre 0 a 255. 

A janela NotFoundError poderá aparecer caso a cor não seja encontrada. 
A janela ValueError poderá aparecer caso um nome inválido de cor seja especificado. </translation>
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

Apaga a cor&quot;name&quot;. Toda a ocorrência dessa cor será substituída pelo substituidor de cor. Se não especificado, a substituição ocorrerá por nenhuma cor - transparente. 

Apagar a cor funcionará nas cores padrões de documento caso nenhum documento esteja aberto. 
Nesse caso, substituir, se especificado, não terá efeito. 

A janela NotFoundError poderá aparecer caso a cor não seja encontrada. 
A janela ValueError poderá aparecer caso um nome inválido de cor seja especificado. </translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;) 

Toda a ocorrência da cor &quot;name&quot; será trocada pela cor substituta.

A janela NotFoundError poderá aparecer caso a cor não seja encontrada. 
A janela ValueError poderá aparecer caso um nome inválido de cor seja espeficicado. </translation>
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

Mostra uma janela de diálogo com título&quot;caption&quot;. Arquivos são filtrados com o filtro de arquivos. Um nome de arquivo padrão ou um caminho de arquivo padrão também pode ser utilizado, deixando-se vazio quando sua utilização não for desejada. Além disso, é possível habilitar a visualização em pequenas imagens na caixa de seleção de arquivo. Esses parametros estão desabilitados por padrão. 

O filtro, se especificado, mostra as extensões de arquivos (*.type *.type2...). 
Por exemplo, imagens (*.png *.xpm *.jpg). 

Faça referência a documentação do QT em QFileDialog para detalhes sobre os filtros. 

Example: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Example: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)</translation>
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

Mostra uma caixa de mensagem com o título &quot;caption&quot;, a mensagem message&quot;,  um ícone icon&quot; e três botões. Por dadrão, nenhum ícone é usado e apenas um botão é mostrado,o botão OK. Apenas o título e a mensagem são requeridos, mas apesar disso, definir um ícone e botões apropriados é altamente recomendado. 

Retorna o número do botão que o usuário pressionou. O número dos botões começam com 1. 

Para parametros de ícone e botões há constantes predefinidas disponíveis com o mesmo nome usado na documentação do QT. Estas  são as constantes BUTTON_* e ICON_* definidas no módulo. Existem duas constantes extras que podem ser  binary-ORed com as constantes do botões. 
    BUTTONOPT_DEFAULT   Pressionando ENTER pressiona esse botão.
    BUTTONOPT_ESCAPE    Pressionando ESC pressiona esse botão. 

Exemplos de Uso:
result = messageBox(&apos;Script failed&apos;,
                    &apos;Este script apenas funciona quando você tem um texto selecionado.&apos;,
                    ICON_ERROR)
result = messageBox(&apos;Macacos!, &apos;alguma coisa de certo! &lt;i&gt;Isso foi um macaco?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE) 

Constantes definidas para boão e ícone:
BUTTON_NONE, BUTTON_ABORT, BUTTON_CANCEL, BUTTON_IGNORE, BUTTON_NO,
BUTTON_NOALL, BUTTON_OK, BUTTON_RETRY, BUTTON_YES, BUTTON_YESALL,
ICON_NONE, ICON_INFORMATION, ICON_WARNING, ICON_CRITICAL. </translation>
    </message>
    <message>
        <source>valueDialog(caption, message [,defaultvalue]) -&gt; string

Shows the common &apos;Ask for string&apos; dialog and returns its value as a string
Parameters: window title, text in the window and optional &apos;default&apos; value.

Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)
</source>
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; string 

Mostra o comum Perguntar para comandos de caixas de diálogos e retorna o valor como um comando:  window title, text in the window and optional &apos;default&apos; valor.


Example: valueDialog(&apos;title&apos;, &apos;text in the window&apos;, &apos;optional&apos;)</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Fecha o documento atual sem perguntar por salvar. 

A janela NoDocopenError pode aparecer se não houver nenhum documento a ser fechado</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; bool 

Retorna verdadeiro caso houver algum documento aberto. </translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;) 

Abre o documento &quot;name&quot;. 

A janela ScribusError pode aparecer caso o documento não possa ser aberto. </translation>
    </message>
    <message>
        <source>saveDoc()

Saves the current document with its current name, returns true if successful.
If the document has not already been saved, this may bring up an interactive
save file dialog.

If the save fails, there is currently no way to tell.
</source>
        <translation>saveDoc() 

Salva o documento atual com o seu nome atual, retorna verdadeiro se bem sucedido. 
Se o documento já não foi salvo, a janela de salvamento aparecerá. 

Se o salvamento falhar, atualmente não a jeito de avisar. </translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;) 

Salva o documento atual com um novo nome &quot;name&quot; (que pode ser uma caminho relativo ou total). 

A janela ScribusError pode aparecer caso o salvamento falhe. </translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool 

Define as informações do documento. Autor, informação, Descrição são valores. </translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br) 

Define as margens do documento, Esquerda (lr), Direia (rr), Topo (tr), Fundo (br). As margens são medidas na unidade do documento - veja em &apos;unidades do documento&apos;. </translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type) 

Mudanças na unidade de medida do documento. Os valores possíveis para unidades estão disponíveis em &apos;unidade do documento&apos;. 

A janela ValueError pode aparecer caso uma unidade inválida seja utilizada. </translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (Scribus unit constant) 

Retorna as unidades de medidado documento. Os valores retornados serão um desses; UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS. </translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;) 

Carrega os estilos de parágrafo do documento do Scribus em &quot;filename&quot; para o documento atual. </translation>
    </message>
    <message>
        <source>setDocType(facingPages, firstPageLeft)

Sets the document type. To get facing pages set the first parameter to
FACINGPAGES, to switch facingPages off use NOFACINGPAGES instead.  If you want
to be the first page a left side set the second parameter to FIRSTPAGELEFT, for
a right page use FIRSTPAGERIGHT.
</source>
        <translation>setDocType(facingPages, firstPageLeft)

Define o tipo de documento, Para utilizar a opção &apos;páginas de rosto&apos; use o parametro FACINGPAGES, sem não mude para NOFACINGPAGES. Se você deseja que a primeira página de seu documento seja à esquerda, defina o parametro FIRSTPAGELEFT, caso contrário use FIRSTPAGERIGHT. 
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; string 

Retorna o nome da cor da linha do objeto &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; integer 

Retorna o comprimento da linha do objeto &quot;name&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; integer 

Retorna o valor de sombreamento da cor da linha do objeto &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants) 

Retorna o estilo de união do objeto &quot;name&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Os tipos de uniões são: 
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants) 

Retorna o valor da terminação de linha.Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Os tipos de terminação são: 
CAP_FLAT, CAP_ROUND, CAP_SQUARE</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants) 

Retorna o estilo de linha do objeto &quot;name&quot;.Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Os estilos de linha são:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Retorna o valor de sombreamento da tinta de preenchimento do objeto &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y) 

Retorna o valor (x, y) contendo a proporção do quadro de imagens &quot;name&quot;.
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; string 

Retorna o nome do arquivo para a imagem no quadro de imagem.
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (width,height) 

Retorna os valores de altura e largura com o tamanho do objeto &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. O tamanho é expresso na unidade de medida do documento - veja em &apos;unidade do documento para referência. </translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;name&quot;]) -&gt; integer 

Retorna a rotação do objeto &quot;name&quot;. Os valores são expressos em graus, e sentido horário. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; list

 Retorna uma lista contendo os nomes de todos os objetos da página atual. </translation>
    </message>
    <message>
        <source>moveObjectAbs(x, y [, &quot;name&quot;])

Moves the object &quot;name&quot; to a new location. The coordinates are expressed in
the current measurement unit of the document (see UNIT constants).  If &quot;name&quot;
is not given the currently selected item is used.  If the object &quot;name&quot;
belongs to a group, the whole group is moved.
</source>
        <translation>moveObjectAbs(x, y [, &quot;name&quot;]) 

Movimenta o objeto &quot;name&quot; para uma nova posição. As coordenadas são expressas na unidade de medida do documento (veja a constante UNIT). Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 
Se o objeto &quot;name&quot; pertencer a um grupo, todo o grupo será movido. </translation>
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

Rotaciona o objeto &quot;name&quot; por &quot;rot&quot; em graus relativos. O objeto é rotacionado pelo vértice atualmente selecionado como ponto de rotação - por padrão, o vértice superior esquerdo quando sem nenhuma rotação. Valores positivos significam sentido anti-horário quando o ponto padrão de rotação é usado. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(width, height [, &quot;name&quot;]

Redimensiona o objeto &quot;name&quot; para a largura e altura dada. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; string

 Retorna o nome do objeto selecionado. &quot;nr&quot; se for dado, indica o número de objetos selecionados, por exemplo 0 significa o primeiro objeto selecionado, 1 o segundo e assim por diante. </translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; integer 

Retorna o número de objetos selecionados. </translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;) 

Seleciona um objeto pelo nome dado. </translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll() 

Desseleciona todos os objetos em todo o documento. </translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list) 

Nomes do grupos de objetos nomeados juntos na &quot;list&quot;. &quot;list&quot; deve conter o nomes dos objetos agrupados. Se &quot;list&quot; não for dado, o ítem atualmente selecionado será usado. 
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;) 

Destroi o grupo pertencente ao objeto &quot;name&quot; . Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado.</translation>
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

Redimensiona o grupo o objeto &quot;name&quot; pertencente. Valores maiores que 1 aumentam o grupo, valores menores que 1 reduz. Por exemplo, o valor de 0.5 redimensiona o grupo para 50% do seu tamanho original. Um valor de 1.6 150%. O valor do &quot;factor&quot; deve ser maior que 0. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela ValueError pode aparecer se um fator de redimensionamento inválido for usado. </translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;]) 

Carrega a imagem &quot;picture&quot; dentro de um quadro de imagem &quot;name&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela WrongFrameTypeError pode aparecer se o quadro alvo não for um quadro de imagem. </translation>
    </message>
    <message>
        <source>scaleImage(x, y [, &quot;name&quot;])

Sets the scaling factors of the picture in the image frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. A number of 1
means 100 %.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>scaleImage(x, y [, &quot;name&quot;]) 

Define o fator de redimensionamento da imagem no quadro de imgem &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Um número de 1 significa 100%. 

A janela WrongFrameTypeError pode aparecer se o quadro alvo não for um quadro de imagem. </translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

Bloqueia o &quot;name&quot; se estiver desbloqueado ou desbloqueia se estiver bloqueado. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Retorna verdadeiro se bloqueado. </translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

 Retorna verdadeiro se o objeto &quot;name&quot; estiver bloqueado. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list 

Retorna uma lista com os nomes de todas as fontes disponíveis. </translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; list of tuples 

Retorna extensa informação sobre a fonte; É uma lista com: 
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]</translation>
    </message>
    <message>
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool 

Cria um visualização em imagem da fonte &quot;name&quot; com o texto &quot;sample&quot; e tamanho. 
A imagem é salva no arquivo &quot;filename&quot;. Retorna verdadeiro se bem sucedido. 

A janela NotFoundError pode aparecer se a fonte especificada não for encontrada. 
A janela ValueError pode aparecer se um exemplo ou nome de arquivo vazio for usado. </translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list 

Retorna uma lista com o nomes de todos as camadas definidas. </translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;) 

Define a camada ativa pelo nome da camada &quot;name&quot;. 

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. 
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string 

Retorna o nome da camada ativa atual. </translation>
    </message>
    <message>
        <source>sentToLayer(&quot;layer&quot; [, &quot;name&quot;])

Sends the object &quot;name&quot; to the layer &quot;layer&quot;. The layer must exist.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>sentToLayer(&quot;layer&quot; [, &quot;name&quot;]) 

Envia o objeto de &quot;name&quot; para a camada &quot;layer&quot;. A camada deve existir.
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>setLayerVisible(&quot;layer&quot;, visible)

Sets the layer &quot;layer&quot; to be visible or not. If is the visible set to false
the layer is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerVisible(&quot;layer&quot;, visible)

Define a camada &quot;layer&quot; para ser visível ou não. Se visível estiver definido como falso a camada está invisível. 
 
A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>setLayerPrintable(&quot;layer&quot;, printable)

Sets the layer &quot;layer&quot; to be printable or not. If is the printable set to
false the layer won&apos;t be printed.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setLayerPrintable(&quot;layer&quot;, printable)

Define a camada &quot;layer&quot; para ser impressa ou não. Se imprimível estiver definido como falso, a camada não será impressa. 

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the Layer &quot;layer&quot; is visible or not, a value of True means
that the layer &quot;layer&quot; is visible, a value of False means that the layer
&quot;layer&quot; is invisible.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool 

Retorna se a camada está ou não visível, um valor verdadeiro indica que a camada está visível, um valor falso indica que ela está invisível.

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>isLayerPrintable(&quot;layer&quot;) -&gt; bool

Returns wether the layer &quot;layer&quot; is printable or not, a value of True means
that the layer &quot;layer&quot; can be printed, a value of False means that printing
the layer &quot;layer&quot; is disabled.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>isLayerPrintable(&quot;layer&quot;) -&gt; bool


 Retorna se a camada &quot;layer&quot; é imprimível ou não, um valor verdadeiro indica que a camada &quot;layer&quot; pode ser impressa, uma valor falso indica que a camada &quot;layer&quot; não pode ser impressa.

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>deleteLayer(&quot;layer&quot;)

Deletes the layer with the name &quot;layer&quot;. Nothing happens if the layer doesn&apos;t
exists or if it&apos;s the only layer in the document.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>deleteLayer(&quot;layer&quot;)

Apaga a camada de nome &quot;layer&quot;. Nada acontece se a camada não existir ou ser a única camada do documento.

A janela NotFoundError pode aparecer se a camada não for encontrada. 
A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer) 

Cria uma nova camada com o nome &quot;name&quot; . 

A janela ValueError pode aparecer se o nome não for aceitável. </translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; string 


Retorna o idioma da interface. </translation>
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

Cria uma nova elipse no documento atual e retorna um nome. 
As coordenadas são dadas na unidade de medida do documento (veja as constantes unidades). &quot;name&quot; deve ser o único identificador do objeto porque  você precisa desse nome para futuras referências. Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. </translation>
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

Cria um novo quadro de imagem na página atual e retorna um nome. As coordenadas são dadas na unidade de media do documento. 
&quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências. Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. </translation>
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

Cria um novo quadro de texto na página atual e retorna um nome. 
As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências.
Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. </translation>
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

Cria uma nova linha do ponto (x1, y1) para o ponto (x2, y2) e retorna um nome. As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências.
Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. </translation>
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

Cria uma nova linha de polígono e retorna um nome. Os pontos para a lihna de polígono são gravadas na lista &quot;list&quot; na seguinte sequência: [x1, y1, x2, y2...xn. yn]. As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências.
Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. 
A janela Value Error pode aparecer se você usar um número insuficiênte de pontos ou se os números utilizados não se agrupam nos pontos sem haver sobras. </translation>
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

Cria um novo polígono e retorna um nome. Os pontos para os polígonos são gravados na lista &quot;list&quot;  na seguinte sequência: [x1, y1, x2, y2...xn. yn].
Ao menos três pontos são requeridos. Não é preciso repetir o primeiro ponto para fechar o polígono. O polígono é automaticamente fechado conectando-se o primeiro e último ponto. As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências. Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. 
A janela Value Error pode aparecer se você usar um número insuficiênte de pontos ou se os números utilizados não se agrupam nos pontos sem haver sobras. </translation>
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

Cria um novo curva de bezier e retorna um nome. Os pontos para a curva de bezier são gravados na lista &quot;list&quot; na seguinte sequência: 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]. 
Na lista de pontos, o x e o y significam as coordenadas x e y do ponto e kx e ky significam o controle de curve para o ponto. As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências. Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. 
A janela Value Error pode aparecer se você usar um número insuficiênte de pontos ou se os números utilizados não se agrupam nos pontos sem haver sobras. </translation>
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

Cria um novo caminho para o texto (path) emergindo dois objetos, caixa de texto e curva de bezier e retorna um nome. As coordenadas são dadas na unidade de medida do documento (veja a constante unidade de medida). &quot;name&quot; deve ser o único identificador do objeto porque você precisa desse nome para futuras referências. Se &quot;name&quot; não for dado, o Scribus criará um para você. 

A janela NameExistError pode aparecer se você usar um nome  que já está sendo utilizado. 
A janela Value Error pode aparecer se você usar um número insuficiênte de pontos ou se os números utilizados não se agrupam nos pontos sem haver sobras. </translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;]) 

Apaga o ítem de nome &quot;name&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Habilita/Desabilita a recurso &apos;texto segue ao redor do quadro&apos;  para o objeto &quot;name&quot;. É chamado com parametros e opcionalmente por &quot;state&quot; boolean. Se &quot;state&quot; não for passado, o camimnho do texto é******************************************</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; bool 

Testa se um objeto com um nome específico realmente existe no documento. 
O parametro opcional é o nome do objeto. Quando nenhum nome de objeto é dado, retorna verdadeiro se há algo selecionado. </translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Aplica o estilo &quot;style&quot; para o objeto de nome &quot;name&quot;. Se nenhum nome de objeto for dado, é aplicado ao objeto selecionado. </translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; list 

Retorna um lista de nomes de todos os estilos de parágrafos no documento atual. </translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; integer 

Retorna o númerio da página de trabalho atual. O números das páginas começam do 1 para cima, não importando a página exibida como primeira do seu documento. </translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Redesenhar todas as páginas. </translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;name&quot;) 

Salva a página atual como EPS para o arquivo &quot;name&quot;. 

A janela ScribusError pode aparecer se o salvamento falhar. </translation>
    </message>
    <message>
        <source>deletePage(nr)

Deletes the given page. Does nothing if the document contains only one page.
Page numbers are counted from 1 upwards, no matter what the displayed first
page number is.

May raise IndexError if the page number is out of range
</source>
        <translation>deletePage(nr) 

Apaga a página dada. Não faz nada se o documento conter apenas um página. 
O números das páginas começam do 1 para cima, não importando a página exibida como primeira do seu documento. 

A janela IndexError pode aparecer se o número da página estiver fora do intervalo. </translation>
    </message>
    <message>
        <source>gotoPage(nr)

Moves to the page &quot;nr&quot; (that is, makes the current page &quot;nr&quot;). Note that
gotoPage doesn&apos;t (curently) change the page the user&apos;s view is displaying, it
just sets the page that script commands will operates on.

May raise IndexError if the page number is out of range.
</source>
        <translation>gotoPage(nr)

Vai para a página &quot;nr&quot; (ou seja, faz a página atual ser a &quot;nr&quot;). Note que gotoPage não (atualmente) muda a página que está sendo mostrada para o usuário, isso apenas define que a página que o comando do script irá operar.

A janela IndexError pode aparecer se o número da página estiver fora do intervalo. </translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; integer 

Retorna o número de páginas no documento. </translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; list

Retorna uma lista contendo o posicionamento das guias. Os valores são dados na unidade de medida do documento - veja a constantes de unidade de medida. </translation>
    </message>
    <message>
        <source>setHGuides(list)

Sets horizontal guides. Input parameter must be a list of guide positions
measured in the current document units - see UNIT_&lt;type&gt; constants.

Example: setHGuides(getHGuides() + [200.0, 210.0] # add new guides without any lost
         setHGuides([90,250]) # replace current guides entirely
</source>
        <translation>setHGuides(list) 

Define as guias horizontais. O parametro de entrada deve ser uma lista das posições da guia na unidade de medida do documento - veja a constante unidade de medida. 

Exemplo: setHGuides(getHGuides() + [200.0, 210.0] # adiciona novas guias sem nenhuma perda 
         setHGuides([90,250]) # substitui todas as guias atuais. </translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides() 

Veja getHGuides. 

</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides() 

Veja setHGuides. 
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; tuple 

Retorna os valores com as dimensões de página dadas pela unidade de medida do documento. 
Veja as constantes unidade de medida e getPageMargins(). </translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; list 

Retorna uma lista de campos com os ítens da página atual. Os campos são: 
(name, objectType, order) Ex. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)] 
significa que o objeto nomeado &apos;Text1&apos; é um quadro de texto (type4) e é o primeiro na página. </translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins() 

Retorna as margens da página como campos (esquerda, direita, topo e fundo) na unidade de medida do documento. Veja a constante unidade de medida do documento e getPageSize(). </translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;]) 

Define a cor de preenchimento do objeto &quot;name&quot; para a cor &quot;color&quot;. &quot;color&quot; é o nome de um das cores definidas. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Define a cor da linha do objeto &quot;name&quot; para a cor &quot;color&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>setLineWidth(width, [&quot;name&quot;])

Sets line width of the object &quot;name&quot; to &quot;width&quot;. &quot;width&quot; must be in the
range from 0.0 to 12.0 inclusive, and is measured in points. If &quot;name&quot; is not
given the currently selected item is used.

May raise ValueError if the line width is out of bounds.
</source>
        <translation>setLineWidth(width, [&quot;name&quot;]) 

Define a espessura da linha do objeto &quot;name&quot; para &quot;width&quot;. &quot;width&quot; deve estar no intervalo de 0.0 a 12.0 e é medido em pontos. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela ValueError pode aparecer se a espessura da linha estiver fora do limite. </translation>
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

Define o sombreamento da cor da linha do objeto &quot;name&quot; para &quot;shade&quot;. 
&quot;shade&quot;  deve ser um valor íntegro no intervalo de 0 (muito iluminado) a 100 (intensidade total da cor). Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela ValueError pode aparecer se a espessura da linha estiver fora do limite. 
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Define o estilo de união de linha do objeto &quot;name&quot; para o estilo &quot;join&quot;.
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Existem constantes predefinidas para uniões - veja em &apos;tipos de união&apos;. 
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;name&quot;]) 

Define o estilo da cobertura de linha do objeto &quot;name&quot; para o estilo &quot;cap&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Existem constantes predefinidas para &quot;cap&quot; - veja em estilos de cobertura. </translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;]) 

Define o estilo de linha do objeto &quot;name&quot; para o estilo &quot;style&quot;. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. Existem constantes predefinidas para &quot;style&quot; - veja em estilo de linhas. </translation>
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

Define o sombreamento da cor de preenchimento do objeto &quot;name&quot; para &quot;shade&quot;. &quot;shade&quot; deve ser um valor íntegro no intervalo de 0 (muito iluminado) para 100 (intensidade total da cor). Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela ValueError pode aparecer de o preenchimento de sombra estiver fora do limite. </translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;name&quot;]) 

Define o grau de raio do canto do objeto &quot;name&quot;. O raio deve estar expresso em pontos. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela ValueError pode aparecer se o raio do canto estiver negativo. </translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Define o estilo de linha do objeto &quot;name&quot; para o estilo de nome &quot;namedStyle&quot;.
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

A janela NotFoundError pode aparecer se o estilo de linha não existir. </translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset() 

Limpa a barra de progresso do Scribus para a configuração anterior. Isto é chamado depois de novo uso da barra de progresso. Veja progessSet. </translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max) 

Define o máximo valor de passos da barra de progresso para um número específico. 
Veja progressSet. </translation>
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

Define  a posição da barra de progressos em &quot;nr&quot;, um valor relativo ao anteriormente definido no progressTotal. A barra de progresso usa o conceito de passos, você dá o número total de passos e o número de passos completados e isso mostrará a porcentagens dos passos que foram completados. Você pode especificar o número total de passos com o progressTotal(). O número atual de passos é definido com o progressSet(). A barra de progresso pode ser reiniciada para os valores padrões com o progressReset() (baseado nas informações retiradas da documentação da Trolltech &gt; QT). </translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor() 

[NÃO SUPORTADO ATUALMENTE] Deixe vazio por enquanto. </translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(bool) 

Habilita/Desabilita ícone de salvamento na barra de ícone do Scribus ou no ítem Salvar do menu. É útil para chamar um procedimento quando você está alterando o documento, porque o Scibur não irá notar automaticamente quando você mudou o documento usando o script. </translation>
    </message>
    <message>
        <source>defineColor(&quot;name&quot;, c, m, y, k)

Defines a new color &quot;name&quot;. The color Value is defined via four components:
c = Cyan, m = Magenta, y = Yellow and k = Black. Color components should be in
the range from 0 to 255.

May raise ValueError if an invalid color name is specified.
</source>
        <translation>defineColor(&quot;name&quot;, c, m, y, k) 

Define um novo nome para a cor &quot;name&quot;. O valor da cor é composto por quatro componentes: C = Ciano, M = Magenta, Y = Amarelo, K = Preto. 
Os componentes da cor devem estar no intervalo de 0 a 255. 

A janela ValueError pode aparecer se uma cor inválida for especificada. </translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getCornerRadius([&quot;name&quot;]) -&gt; integer 

Retorna o raio cdo canto do objeto &quot;name&quot;. O raio é expresso em pontos. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. </translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation>getPosition([&quot;name&quot;]) -&gt; (x,y) 

Retorna os campos (x, y) com a posição do objeto &quot;name&quot;. 
Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 
A posição é expressa nos valores atuais da unidade de medida do documento. Veja as constantes unidade de medida para referência. </translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>rotateObjectAbs(rot [, &quot;name&quot;]) 

Define a rotação do objeto &quot;name&quot; para &quot;rot&quot;. Valores positivos significam sentido anti-horário de rotação. Se &quot;name&quot; não for dado, o ítem atualmente selecionado será usado. 

</translation>
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
        <source>selectText(start, count, [&quot;name&quot;])

Selects &quot;count&quot; characters of text in the text frame &quot;name&quot; starting from the
character &quot;start&quot;. Character counting starts at 0. If &quot;count&quot; is zero, any
text selection will be cleared. If &quot;count&quot; is -1, all text in the frame will
be selected. If &quot;name&quot; is not given the currently selected item is used.

May throw IndexError if the selection is outside the bounds of the text.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>register_macro_code(name, sourcetext, accel=&apos;&apos;)

Create a macro named &quot;name&quot; by evaluating the the source code &quot;sourcetext&quot;.
&quot;sourcetext&quot; must follow the same rules as macros created in the GUI.
If provided, the string &quot;accel&quot; will be used to set a keyboard shortcut
for the macro.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>Build-ID:</source>
        <translation>ID da Build:</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation>Versão do Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation>%1. %2 %3</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation>Galego:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation>Tcheco:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation>Galês:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>Contribuições de:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation>Polonês:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation>Ucraniano:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation>Eslovaco:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation>Italiano:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation>Dinamarquês:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation>Lituano:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation>Turco:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation>Húngaro:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation>Francês:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation>Búlgaro:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation>Alemão:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation>Windows port:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation>Norueguês:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation>Russo:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation>Português do Brasil:
</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation>Finlandês:</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Sobre</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>A&amp;utores</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation>Esloveno:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation>Básco:</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Traduções</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Online</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Esse painel mostra a versão, data de consturção e 
bibliotecas compiladas que o Scribus suporta 
O C-C-T  é igual a C=CUPS C=littlecms T=TIFF suporte. 
Bibliotecas não suportadas são indicadas por a *</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation>Suíço:</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Time de Desenvolvimento:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Documentação Oficial:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Outras Documentações:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation>Inglês (Britânico):</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Homepage</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referência Online</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Bugs e Requisições de Recursos</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Lista de Discussões</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Traduções Oficiais e Tradutores:
</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation>Esperanto:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation>Koreano:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation>Sérvio:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation>Espanhol:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Contribuições de Tradução Anteriores:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation>Calatão:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Sobre o Scribus %1</translation>
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
        <translation>Ooções Avançadas</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation>Cria PostScript de nível 3</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation>Cria apenas PostScript de nível 2, fique atento, 
isso cria arquivos imensos</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation>Cria apenas PostScript de nível 1, fique atento, 
isso cria arquivos imensos</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation>Espelhar Página(s) Horizontalmente</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation>Espelhar Página(s) Verticalmente</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation>Aplicar Perfis &amp;ICC</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation>PostScript nível &amp;1</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation>PostScript nível &amp;2</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation>PostScript nível &amp;3</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation>Aplicar &amp;Remoção de Sobre Cor (UCR)</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>O UCR desabilita sombras que são compostas por 
ciano, amarelo e magenta é utiliza preto no lugar. 
No UCR a maior parte das cores afetadas são imagens 
que são neutras e/ou de tons escuros que são compostos 
por cinza. A utilização desse modo melhora a impressão 
de algumas imagens e alguns experimentos e testes são 
necessários de caso para caso. O UCR reduz a 
possibilidade de saturação com as cores CMY.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source> p</source>
        <translation>paicas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>Align</source>
        <translation>Alinhar</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation>Lado Superior</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation>Lado Direito</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation>Lado Esquerdo</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation>Lado Inferior</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Distribute/Align</source>
        <translation>Distribuir/Alinhar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation>&amp;Aplicar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation>&amp;Entre:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation>A&amp;linhar</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation>Su&amp;bstituição</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation>Distribuir &amp;Igualmente</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation>Ent&amp;re:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation>&amp;Não Alterar</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation>Al&amp;inhar</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation>Su&amp;bstituição</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation>Distribuir Igual&amp;mente</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation>&amp;Não Alterar</translation>
    </message>
</context>
<context>
    <name>AlignSelect</name>
    <message>
        <source>Align Text Left</source>
        <translation>Alinhar texto a esquerda</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Alinhar texto a direita</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Alinhar texto ao Centro</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Texto Justificado</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Forçar Texto Justificado</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>soma</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Pressionar</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Star</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Fino</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Horário</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Grosso</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Acrescentar a frente do Símbolo da Moeda</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Enviar Formulário</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Limite de</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Verificar</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Cruzado</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>On Focus</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Ir Para</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Ícones</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Inserção</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nome:</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Outro</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Texto Simples</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Fonte:</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Caixa de Lista</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Usar Símbolo de Moeda</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Formato de Número</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Verificar Estilo:</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Para Arquivo:</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Não Imprimir</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Enviar para Endereço:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Calcular</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Apontado</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Valor não foi validado</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>máximo</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Formatando</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Caixa de Combinação</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Script de validação personalizado:</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>mínimo</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Ação</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Borda</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Formato de Data</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botão</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Círculo</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Cor:</translation>
    </message>
    <message>
        <source>Field Properties</source>
        <translation>Propriedades do Campo</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Pontilhado</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Formato da Data</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Valor é o</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Formato</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Oculto</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Script de cálculo personalizado:</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Scripts Personalizados</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Invisível</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Número</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Remover</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Esquadrinhado</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Estilo:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>X-Pos:</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Campo formatado como:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Y-Pos:</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation>Java Script</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Largura:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Dica de Ferramenta:</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>On Blur</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Não Exportar Valores</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Enviar Data como HTML</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de Texto</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Posicionamento do ícone...</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Mouse Up</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Selecionar...</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Mouse Enter</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Usar ícones</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Somente Leitura</translation>
    </message>
    <message>
        <source>product</source>
        <translation>produto</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Texto do Botão Apontado</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Mudar...</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Formato da Porcentagem</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Caracteres</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Password</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Visibilidade:</translation>
    </message>
    <message>
        <source>average</source>
        <translation>média</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Bizotado</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Importar Dados</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Mouse Exit</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Mouse Down</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Script:</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Porcentagem</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Decimais:</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>dos seguintes campos:</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Importar Dados de:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation> e menor ou igual a: </translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Keystroke</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Multi-Line</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Sublinhado</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Diamante</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Editável</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Aparência</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Caixa de Checagem</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Valor não foi calculado</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Editar...</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Reiniciar Formulário</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Requerido</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Não Verifique Ortografia</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Cotornar</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Visível</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Valor deve ser maior ou igual a:</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Seleção</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Default foi Selecionado</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Exemplo:</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Não Exibir Rolagem</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Texto do Botão Pressionado</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Necessário o ícone para Normal, para usar ícones para Botões</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Selection Change</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Pressionado</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Página:</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Entre com uma lista de campos separados por virgulas aqui</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation>Imagens (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Fonte para utilizar com PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Flag é ignorado no PDF 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Arquivos PDF (*.pdf);;Todos os Arquivos (*)</translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Destino</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ligação</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Annotation Properties</source>
        <translation>Annotation Properties</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>PDF- (*.pdf);;Todos (*)</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Ligação Externa</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Link(web) Externo</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Tipo:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Alte&amp;rar...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Página:</translation>
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
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Apply Template</source>
        <translation>Aplicar Modelo</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Modelo:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation>Aplicar a página &amp;atual</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation>Aplicar da &amp;página:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation>a:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation>Aplicar a todas as páginas &amp;regulares</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation>Aplicar a todas as páginas &amp;inversas</translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Delete</source>
        <translation>Apagar</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Objeto</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Scrapbooks (*.scs);;All Files (*)</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Galeria</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Nova Entrada</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>&amp;Carregar...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salvar &amp;Como...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Pequeno</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Médio</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Grande</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Visualizar</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Renomear</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome  &quot;%1&quot; não é único. 
Por favor escolha outro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nome:</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Move Bookmark</source>
        <translation>Mover Favorito</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Bookmarks</source>
        <translation>Favoritos</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Inserir Favorito</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Favoritos</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Ícone</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Nunca</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Reiniciar</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Título sobrescreve Ícone</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Proporção como:</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Aparência:</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Título somente</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Título abaixo do ícone</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Sempre</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Proporção:</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Ícone Somente</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Quando o ícone for muito pequeno</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Título á esquerda do ícone</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Título á direita do ícone</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Quando o ícone for muito grande</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Sem proporção</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Proporcional</translation>
    </message>
    <message>
        <source>Icon Placement</source>
        <translation>Posicionamento do ícone</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Título acima do ícone</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrico Absoluto</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Perfis Icc do Sistema</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Rendering Intents</translation>
    </message>
    <message>
        <source>Color Management Settings</source>
        <translation>Conimagemr Gerenciamento de Cor</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Perceptual</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrico Relativo</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturação</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation>Perfil de cor padrão para imagens importadas</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Perfil de cor padrão para cores sólidas na página </translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Perfil de cor que você gerou ou recebeu do fabricante. 
Esse perfil deve ser específico de seu monitor e não um perfil genérico, por exemplo sRGB.</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Perfil de cor do modelo de sua impressora. 
Isto deve ser específico da sua impressora e não um perfil genérico, por exemplo sRGB.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Compensação do Ponto Preto é o método de melhorar o contraste em fotos. 
É recomendável você habilitar essa opção caso seu documento possua fotos.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Gerenciamento de Cor &amp;Ativo</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation>&amp;Imagens:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>Cores &amp;Sólidas:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Monitor:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>&amp;Impressora:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>M&amp;onitor:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>&amp;Impressora:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Sum&amp;ular Impressão na Tela</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Marca as cores fora da &amp;Gama</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Usar Compensação de &amp;Ponto Preto</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Renderização padrão pretendida para seu monitor. Ao menos que você saiba porque mudar isso, Colormetria Relativa ou Percentual deve ser selecionada.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Renderização padrão pretendida para sua impressora. Ao menos que você saiba porque mudar isso, Colormetria Relativa ou Percentual deve ser selecionada.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Habilita prova de cor via software. Tenta simular como as cores serão impressas a partir de um perfil de impressora.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Método de exibição na tela talvez não será impresso corretamente. 
Isto requer um perfil altamente apurado e serve apenas como aviso.</translation>
    </message>
</context>
<context>
    <name>CMYKChoose</name>
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
        <translation>OK</translation>
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
        <translation>Nova</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Antigo</translation>
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
        <source>Web Safe RGB</source>
        <translation>RGB Seguro para Web</translation>
    </message>
    <message>
        <source>Edit Color</source>
        <translation>Editar Cor</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Barras de Cor Estáticas</translation>
    </message>
    <message>
        <source>Dynamic Color Bars</source>
        <translation>Barras de Cor Dinâmicas</translation>
    </message>
    <message>
        <source> %</source>
        <translation> % </translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>Mapa de Cores HSV</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>O nome da Cor não é único</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nome:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>&amp;Modelo de Cor</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Você não pode criar a cor de nome &quot;%1&quot;. 
Já é um nome reservado por uma cor transparênte</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source> %</source>
        <translation> % </translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Gradiente Radial</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Gradiente Horizontal</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Sólido</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Opacidade:</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Gradiente Vertical</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Gradiente Através da Diagonal</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Gradiente Diagonal</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Gradação:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Editar Propriedades de Cor da Linha</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Editar Propriedades de Cor do Preenchimento</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturação da cor</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Método de Preenchimento Sólido ou Gradiente</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Definir transparência para a cor selecionada</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation>Cor do Objeto Selecionado</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Gradiente Linear Livre</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Gradiente Radial Livre</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
</context>
<context>
    <name>CsvDialog</name>
    <message>
        <source>CSV Importer Options</source>
        <translation>Opções de Importação CSV</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Delimitador de Campo:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(TAB) </translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Delimitador de Valor:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Primeira linha é a líder</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>No</source>
        <translation>Não</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Somente Páginas Pares</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Página por Folha</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Espelho</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Orientação</translation>
    </message>
    <message>
        <source>Printer Options</source>
        <translation>Opções de Impressora</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Somente Páginas Ímpares</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Page Set</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Páginas por Folha</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Impressão N-Up</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Todas Páginas</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Esse painel mostra várias opções do CUPS durante a impressão. 
Os parametros exatos disponíveis vão depender do seu drive de impressora. 
Você pode confirmar o suporte ao CUPS selecionando AJuda &gt; Sobre. 
Veja na lista: C-C-T Isto é igual a C=CUPS C=littlecms T=TIFF suporte. 
Bibliotecas sem suporte são indicadas pelo a *</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Codificação:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Diretório Padrão de Documentos. 
Isso pode ser definido nas Preferências.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Compactar Arquivo</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>&amp;Incluir Fontes</translation>
    </message>
</context>
<context>
    <name>DelColor</name>
    <message>
        <source>?</source>
        <translation>?</translation>
    </message>
    <message>
        <source>Delete Color</source>
        <translation>Apagar Cor</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation>Substituí-la por:</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation>Apagar Cor:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>DelPages</name>
    <message>
        <source>to:</source>
        <translation>até:</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation>Apagar de:</translation>
    </message>
    <message>
        <source>Delete Pages</source>
        <translation>Apagar Páginas</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Use</source>
        <translation>Usar</translation>
    </message>
    <message>
        <source>instead</source>
        <translation> ao invés de</translation>
    </message>
    <message>
        <source>Missing Font</source>
        <translation>Fonte Ausente</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation>A Fonte %1 não está instalada.</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Informações do Documento</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Título:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Autor:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Palavras-chave:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>Descri&amp;ção:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>P&amp;ublicador:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Contribuidores:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>Dat&amp;a:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>T&amp;ipo:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>F&amp;ormato:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>Identi&amp;ficador:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>&amp;Fonte:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>&amp;Relação:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>Cobe&amp;rtura:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>Dir&amp;eitos:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation>&amp;Documento</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Maiores &amp;Informações</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>A pessoa ou organização primariamente responsável 
por fazer o conteúdo do documento. 
Este campo pode ser incorporado em um documento 
do Scribus como referência, como também na 
metadata do PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation>Um nome dado ao documento. 
Este campo pode ser incorporado em um documento 
do Scribus como referência, como também na 
metadata do PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation>Uma descrição do conteúdo do documento. 
Este campo é uma pequena descrição ou abstração 
do documento. É incorporado ao PDF na exportação</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation>O tópico do conteúdo do documento. 
Este campo é para citações (palavras chaves) que 
você gostaria de incorporar ao PDF para o ajuda-lo 
em buscas e na indexação dos arquivos PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Uma pessoa ou organização responsável por 
disponibilizar o documento</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Uma pessoa ou organização responsável por 
 fazer contribuições para o conteúdo do documento </translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Uma data associada com um evento no ciclo 
de vida do documento, no formato AAAA-MM-DD, 
como no ISO 8601 ou DD-MM-AAAA na norma ABNT 
(PRECISO DE CONFIRMAÇÃO)</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>A natureza ou gênero do conteúdo do 
documento, exemplo categorias, funções, gêneros</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation>A manifestação física ou digital do documento. 
Tipo de mídia e dimensões são valores que valem 
apena serem citados. 
RFC2045,RFC2046 para tipos de arquivos são também 
de utilidade aqui</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Uma referênciação clara para o documento 
que dá um contexto como ISBN, URI ou ABNT</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Uma referênciação para a normativa cujo o 
presente documento é derivado, por exemplo 
ISBN, URI ou ABNT</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation>O idioma cujo o conteúdo do documento foi 
escrito, usualmente um código de idioma em 
ISO-639 opcionalmente sufixado com hífen e 
um código ISO-3316, por exemplo en-GB, fr-CH 

</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Uma referência a um documento relacionado, 
possívelmente usando um identificador formal 
como ISBN, URI ou ABNT</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>O escopo do conteúdo do documento, possívelmente 
incluindo localização, hora e jurisdição pertencente</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Informações sobre direitos devem estar dentro ou 
sobre o documento, exempo direitos autorais, 
patentes, marca registrada etc</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>All</source>
        <translation>Todas</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation>Arquivos PostScript (*.ps);; Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Setup Printer</source>
        <translation>Confiruração da Impressora</translation>
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
        <translation>Amarelo</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Preto</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserir lista separada por vírgulas de marcas 
onde notas podem ser encontradas. Use * para 
todas as páginas, 1-5 para um intervalo de 
páginas e apenas um número para um página 
específica.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Impressora de Destino</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Opções...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Arquivo:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Alte&amp;rar...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>Comando A&amp;lternativo de Impressão</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>Co&amp;mando:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Intervalo</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Imprimir &amp;Todas</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Imprimir Pá&amp;gina Atual</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Imprimir &amp;Intervalo</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>N&amp;úmero de Cópias:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation>Imprimir &amp;Normal</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation>Imprimir &amp;Separadores</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation>Impr&amp;imir em Cores se Disponível</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation>Imprimir em Tons de Cinza</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation>Opções A&amp;vançadas...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Imprimir</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Utilize um gerenciador de impressão alternativo 
como Kprinter ou Gtklp, para utilizar opções 
adicionais de impressão</translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Importando Arquivo: 
%1
falhou!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Erro Fatal</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
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
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>O Nome do Estilo não é unico</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Caractere</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation>Espaços Verticais</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Entrelinhas</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation>Efeito:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Tabuladores e Entalhos</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Nome do seu estilo de parágrafo</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fonte de texto selecionado ou objeto</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da fonte</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Cor de preenchimento do texto</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Cor da borda do texto</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation>Ignorar todas as maiúsculas</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Determina o tamanho em número de linhas 
da primeira letra do parágrafo</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation>Alinhar texto a Linha Base da Grelha</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Espaço acima do parágrafo</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Espaço abaixo do parágrafo</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nome:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation>&amp;Fonte:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation>Ta&amp;manho:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation>&amp;Alinhamento:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation>&amp;Letra Inicial</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Linhas:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Cor de P&amp;reenchimento:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation>Cor de L&amp;inha:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation>Ajustar à Linha Base da Grelha</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation>&amp;Entrelinhas:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation>Acim&amp;a:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation>&amp;Abaixo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
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
        <translation>Javascripts (*.js);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salvar &amp;Como...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Salvar e Sair</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Sair sem Salvar</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Colar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Obter Nomes dos Campos</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Selecione um Diretório para a Exportação</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Todas as Páginas</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Mudar o diretório de saída</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>O diretório de saída - o lugar aonde são guardadas 
suas imagens. 
O nome do arquivo exportado será: &apos;nomedodocumento-númerodapágina.tipodearquivo&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Exportar somente página atual</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Formatos Disponíveis para Exportação</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Al&amp;terar...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Exportar para Diretório:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>T&amp;ipo de Imagem:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualidade:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Exportar como Imagem(ns)</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolução:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation> dpi</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Intervalo</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>&amp;Página Atual</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Intervalo</translation>
    </message>
    <message>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Exportar um intervalo de páginas</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserir lista separada por vírgulas de marcas 
onde notas podem ser encontradas. Use * para 
todas as páginas, 1-5 para um intervalo de 
páginas ou apenas um número para um página 
específica.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Exportar todas páginas</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Resolução das Imagens 
Use 72 DPI para imagens que serão exibidas em Tela</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Qualidade de suas imagens - 100% é a 
melhor, 1% é a pior qualidade</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamanho:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Tamanho das Imagens: 100% para tamanho 
real, 200% para imagem duas vezes maior etc.</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Tamanho:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Sem Título</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Autor:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Desconhecido</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Documento Scribus</translation>
    </message>
</context>
<context>
    <name>Farbmanager</name>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Cores</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Configurações das Cores</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Configuração da Cor Atual:</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Definir set de Cor para carregar</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Salvar set de Cor atual</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Remover cores não utilizadas no set de cor do documento atual</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation>Adicionar cores ao set atual de um documento existente</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Criar uma cor nova dentro do ser atual</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Editar a cor selecionada</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Fazer uma cópia da cor selecionada</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Apagar cor a selecionada</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Definir o set de cor atual como set de cor default</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Escolha um nome</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Nova cor</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anexar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>&amp;Remover Inutilizados</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Definir Cor &amp;Salva</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nome:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Nome da fonte</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Substituições de Fontes</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation>Sub-sistema</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Reposição</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation>Inclusa em:</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation>Usar Fonte</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolha um diretório</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation>Postscript</translation>
    </message>
    <message>
        <source>Global Font Settings</source>
        <translation>Configurações Gerais de Fontes</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Caminhos Adicionais</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Fontes Disponíveis</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation>Localização da Fonte</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>Fontes &amp;Disponíveis</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>Substituições de Fonte</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>A&amp;lterar...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>A&amp;dicionar...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Remover</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>&amp;Caminhos Adicionais</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
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
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation>Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Pijamas de seda trocados para quartzo azul</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation>Visualizar Fontes</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation>Anexar fonte selecionada ao Estilo, menu de Fonte</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation>Sair da visualização</translation>
    </message>
</context>
<context>
    <name>GradientEditor</name>
    <message>
        <source>Position:</source>
        <translation>Posição:</translation>
    </message>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Aqui você pode adicionar, alterar ou remover 
cores de sua gradiência.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Guias Horizontais</translation>
    </message>
    <message>
        <source>Manage Guides</source>
        <translation>Gerenciar Guias</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Guias Verticais</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Adicionar</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>D&amp;eletar</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>A&amp;dicionar</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>De&amp;letar</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Bloquear Guias</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Desculpe, nenhum manual disponível! Por favor veja: http://docs.scribus.net para documentação atualizada e www.scribus.net para downloads.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Conteúdo</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Link</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Ajuda OnLine do Scribus</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Skip</source>
        <translation>Skip</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Aceitar</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Hifenação Possível</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation>Configurações do Hifenador</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Idioma:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Menor Palavra:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Largura da menor palavra a ser hifenada.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Número máximo de hifenações seguidas possíveis. 
O valor 0 significa hifenações ilimitadas.</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>Sugestões de &amp;Hifenação</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Hifenação Automática do Texto &amp;Durante a Digitação</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Uma caixa de diálogo mostrando todos os Hífens 
possíveis para cada palavra, aparecerá quando você 
utilizar o Extra, Opção de Hifenação de Texto.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Habilitar hifenação automática em seu texto enquanto digita.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>Hifenação Permitida:</translation>
    </message>
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ao Final</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes da Página</translation>
    </message>
    <message>
        <source>Insert Page</source>
        <translation>Inserir Página</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation>Inserindo</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>após Página</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation>Modelo (Pagina Direita):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation>&amp;Inserindo</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Página(s)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation>&amp;Modelo (Página Esquerda):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation>&amp;Modelo:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>InsertTable</name>
    <message>
        <source>Insert Table</source>
        <translation>Inserir Tabela</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation>Número de Linhas:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation>Número de Colunas:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>JavaDocs</name>
    <message>
        <source>New Script</source>
        <translation>Novo Script</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Editar Javascripts</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Editar...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Adicionar...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Novo Script:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sim</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Você realmente deseja apagar esse Script?</translation>
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
        <translation>Ação</translation>
    </message>
    <message>
        <source>Shift+</source>
        <translation>Shift+</translation>
    </message>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation>Gerenciar Teclas de Atalho</translation>
    </message>
    <message>
        <source>ALT+SHIFT+T</source>
        <translation>ALT+SHIFT+T</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Selecione uma Tecla para esta Ação</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Tecla Atual</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Sem Tecla</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Teclas Definidas pelo &amp;Usuário</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>Definir &amp;Tecla</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Essa sequência de teclas e já está em uso</translation>
    </message>
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Delete Layer</source>
        <translation>Apagar Camada</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation>Abaixar Camada</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation>Camadas</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation>Elevar Camada</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation>Nova Camada</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation>Adicionar uma nova Camada</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation>Você deseja apagar todos Objetos desta camada também?</translation>
    </message>
</context>
<context>
    <name>LineFormate</name>
    <message>
        <source>Edit Line Styles</source>
        <translation>Editar Estilos de Linhas</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Novo Estilo</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copia de %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anexar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sim</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Você realmente deseja apagar esse estilo?</translation>
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
        <translation>pt </translation>
    </message>
    <message>
        <source>mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source>in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Novo</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Close this dialog.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Return to Scribus.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="unfinished">Descrição</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="unfinished">&amp;Editar...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Apagar</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicação Múltipla</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Número de Cópias:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>Translação &amp;Horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>Translação &amp;vertical:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Distâncias</translation>
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
        <translation>Ângulo:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Comprimento:</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation>Erro de Script</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Se você estiver rodando um script oficial, transpásse-o para &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; por favor.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Essa mensagem está em sua área de transferência também. 
Use Ctrl+V para cola-la dentro do rastreador de Bug.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Mostrar &amp;Terminal</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation>Ocultar &amp;Terminal</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla .sla.gz .scd .scd.gz);;All Files (*)</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Imporar</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla ,scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Alterar...</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation>Importar Modelo</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar Página(s)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation>Do Documento:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation>Importar Página(s):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserir lista separada por vírgulas de marcas 
onde notas podem ser encontradas. Use * para 
todas as páginas, 1-5 para um intervalo de 
páginas e apenas um número para um página 
específica.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation> de 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Criar Página(s)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>Página Anterior</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>Página Seguinte</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ao Final</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation> de %1</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>to:</source>
        <translation>para:</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Mover Página(s):</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>ao Final</translation>
    </message>
    <message>
        <source>Move Pages</source>
        <translation>Mover Páginas</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Copiar Página</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>antes da Página</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>depois da Página</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>Nível</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colorimétrico Absoluto</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Sem Estilo</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Distância da curva:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Rendering Intent:</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da fonte</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome &quot;%1&quot; já existe.
Por favor escolha outro.</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Ponto Esquerdo</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation>Espaçamento Personalizado</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Esquadrinhar Pontas</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Mostrar Curva</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Manual Kerning</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Percepção</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colorimétrico Relativo</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>União bizotada</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>União Seca</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Geometria</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturação</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Inserir Perfil:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Distância do texto</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Inicio de deslocamento:</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Ponto Base:</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Forma:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Gradação:</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Terminação</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arredondar Uniões</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Achatar Pontas</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arredondar Pontas</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Linhas da Célula</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Linha no topo</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Linha á esquerda</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Linha á direita</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Linha na Base</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Nome do objeto selecionado</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Posição horizontal da ponto base atual</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Posição vertical da ponto base atual</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Largura</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Altura</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Rotação de objeto no ponto base atual</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Ponto de referência para medição e rotação</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Selecionar canto superior esquerdo como ponto base</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Selecionar canto superior direito como ponto base</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Selecionar canto inferior esquerdo como ponto base</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Selecionar canto inferior direito como ponto base</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Selecionar centro como ponto base</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Inverter Horizontalmente</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Inverter Verticalmente</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Mover um nível acima</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Mover um nível abaixo</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Mover para frente</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Mover para trás</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Travar ou Destravar um objeto</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Travar ou Destravar o tamanho de um objeto</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Habilitar ou Desabilitar impressão de um objeto</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Fonte de texto selecionado ou objeto</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Dimensão da largura de caracteres</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Cor da borda do texto</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Cor de preenchimento do texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturação de cor da borda do texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturação de cor do preenchimento de texto</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Entrelinhas</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo do parágrafo atual</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Alterar configuração para esquerda ou final de objetos</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Modelo da linha</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Espessura da Linha</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Tipo de união de linha</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Tipo da terminação da linha</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Estilo de linha do objeto atual</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Selecione a forma do quadro...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Editar forma do quadro...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Conimagemr radiando do arredondamento de canto</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Número de colunas no quadro de texto</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Distância entre colunas</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Distância do texto da borda superior do quadro</translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Distância do texto da borda inferior do quadro</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Distância do texto da borda esquerda do quadro</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Distância do texto da borda direita do quadro</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Editar configurações de tabulação do quadro de texto...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Permitir que a imagem seja de tamanho diferente do quadro</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Deslocamento horizontal da imagem dentro do quadro</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Deslocamento vertical da imagem dentro do quadro</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Redimensionar a imagem horizontalmente</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Redimensionar a imagem verticalmente</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Manter a proporção X e Y iguais</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Posicionar a imagem de acordo com o tamanho do quadro</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Usar proporções de imagens e não a do quadro </translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Propriedades do Path do Texto</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Indica o nível aonde o objeto está posicionado, 0 significa que o objeto está no fundo</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Faz com que o texto fique em quadros abaixos e 
siga a forma do objeto</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Alternar entre Distância ou Tamanho da Coluna</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Manter Proporção</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Fonte do perfil da imagem</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Rederização pretendida para imagens</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Largura da Coluna</translation>
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
        <translation>&amp;Imagem 
</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Linha</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Cores</translation>
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
        <translation>&amp;Largura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotação:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Editar Forma...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>Cantos 
Aredondad&amp;os:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;nas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Espaçamento:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Topo:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Fundo:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Esquerda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Direita:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>T&amp;abuladores...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Textos &amp;Segue Através do Quadro</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Usar Caixa &amp;Limite</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>&amp;Usar Linha de Contorno</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Tamanho de &amp;Fonte:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation>&amp;Kerning: </translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation>Espaçamento de &amp;Linha:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>Est&amp;ilo:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>Lín&amp;gua:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>Redimencionamento &amp;Livre</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Esc&amp;ala-X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>&amp;Proporção-Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Redimencionar &amp;para o Tamanho do Quadro</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>P&amp;roporcional</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>Ponto &amp;Base:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>T&amp;ipo de Linha:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Espessura de Linha:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>Bor&amp;das:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Terminações:</translation>
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
        <translation>Usar caixa envolvente no lugar do quadro de forma para caminho do texto</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Use uma segunda linha originalmente criada no quadro da forma para caminho do texto</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Hifenar idioma do quadro</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Texto da Direita para Esquerda</translation>
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
        <translation>OK</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>pt </translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Achatar Pontas</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Linha Pontilhada</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Arredondar Uniões</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Espessura da linha:</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome &quot;%1&quot; já existe.Por favor escolha outra.</translation>
    </message>
    <message>
        <source>Edit Style</source>
        <translation>Editar Estilo</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Esquadrinhar Ponta</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Linha separada</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Linha pontuada</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>União Bizotada</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>União Seca</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Arredondar Pontas</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Dash Dot Line</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Linha Sólida</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Cópia de %1</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Atenção</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation>Novo Modelo</translation>
    </message>
    <message>
        <source>Edit Templates</source>
        <translation>Editar Modelo</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nome:</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copiar #%1 de </translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anexar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Não</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sim</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Nome:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation>Você realmente deseja apagar esse Modelo?</translation>
    </message>
</context>
<context>
    <name>NewDoc</name>
    <message>
        <source>Points (pts)</source>
        <translation>Pontos (pts)</translation>
    </message>
    <message>
        <source>New Document</source>
        <translation>Novo Documento</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizar</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Opções</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Margens das Guias</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (P)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Polegadas (pol)</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation>Guias de Coluna</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamanho:</translation>
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
        <translation>Tablóide</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Orientação:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>Páginas de &amp;Rosto</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>&amp;Página Esquerda Primeiro</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Esquerda:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Direita:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Superior:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Inferior:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Número da &amp;Primeira Página:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Unidade &amp;Padrão:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>Quadros de Texto &amp;Automático</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Espaçamento:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;nas:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Tamanho Padrão de Página, comum ou personalizado</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Orientação da páginas do documento</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Largura da página do documento, editável caso 
você tenha selecionado um tamanho personalizado</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Altura da página do documento, editável caso 
você tenha selecionado um tamanho personalizado</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Habilita visão simples ou extendida</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Faz a primeira página à esquerda do documento</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distância entre a margem superior e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distância entre a margem do fundo e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem esquerda e a borda da página. 
Se &apos;Páginas de Rosto&apos; estiver habilitado, esse espaço da 
margem pode ser usado para atingir o espaçamento 
correto para a união de páginas
</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem direita e a borda da página. 
Se &apos;Páginas de Rosto&apos; estiver habilitado, esse espaço da 
margem pode ser usado para atingir o espaçamento 
correto para a união de páginas
</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Número da primeira página do documento</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidade de medida padrão para edição do documento</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Criar quadros de texto automáticamente quando 
novas páginas forem adicionadas</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Número de colunas a criar em quadros de textos 
automaticamente criados</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Distância entre colunas automáticamente criadas</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interna:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>Ext&amp;erna:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Executivo</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Fólio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Livro de Contas</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation>Página Esquerda</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation>Página Direita</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Editor de Nó</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Reiniciar esse Ponto de Controle</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Apagar Nó</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Adicionar Nó</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Mover Nó</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Mover Pontos de Controle</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Reiniciar Pontos de Controle</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation>Quando selecionado, usará as coordenadas relativas a Página,
caso contrário as coordenadas serão relativas ao objeto.</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>Coordenadas &amp;Absolutas</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>&amp;X-Pos:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>&amp;Y-Pos</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Editar Linha de &amp;Contorno</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>&amp;Reiniciar Linha de Contorno</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>Terminar &amp;Edição</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Mover Pontos de Controle de forma Independente </translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Mover Pontos de Controle de forma Simétrica</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Abrir um Polígono ou Cotar uma Curva de Bezier</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>Fechar Curva de Bezier</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Espelhar o Path Horizontalmente</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Espelhar o Path Verticalmente</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Inclinar o Path Horizontalmente para Esquerda</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Inclinar o Path Verticalmente para Cima</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Inclinar o Path Verticalmente para Baixo</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Rotacionar o Path no sentido anti-horário
</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Rotacionar o Path no sentido Horário </translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Reduzir Tamanho do Path e mostrar em %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Aumentar Tamanho do Path e mostrar em %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Ângulo de Rotação</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% para redimensionar </translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Ativar Modo de Edição da linha de Contorno</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Reiniciar a Linha de Contorno para a Forma Original do Quadro</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Inclinar o Path Horizontalmente para Direita</translation>
    </message>
</context>
<context>
    <name>PConsole</name>
    <message>
        <source>Script Console</source>
        <translation>Terminal de Scripts</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Box</source>
        <translation>Caixa</translation>
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
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation>Imagens:</translation>
    </message>
    <message>
        <source>Split</source>
        <translation>Distribuir</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation>Dissolver</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation>Horizontal</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Colormetria Absoluta</translation>
    </message>
    <message>
        <source>Export Range</source>
        <translation>Exportar Intervalo</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation>Opções de Arquivo</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation>Margem Direita</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation>Fontes para incluir:</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation>Esquerda para Direita</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation>Direita para Esquerda</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation>Cores Sólidas:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation>Renderização Pretendida:</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation>Cortinas</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation>Sem Efeito</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation>Fontes Disponíveis:</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation>Dentro</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation>Margem Esquerda</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation>Fora</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation>Impressora</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation>Incluindo</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation>Caixa de Adornos</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation>Perfil:</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Compreensivo</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Colormetria Relativa</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation>Senhas</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation>Saída Pretendida PDF/X-3 </translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efeitos</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Saturação</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation>Vertical</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation>Configurações</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation>Iluminar</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation>De baixo para cima</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation>De cima para baixo</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation>Configurações de Imagem </translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automático</translation>
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
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation>Máxima</translation>
    </message>
    <message>
        <source>High</source>
        <translation>Alta</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Média</translation>
    </message>
    <message>
        <source>Low</source>
        <translation>Baixa</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation>Mínima</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation>Diagonal</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation>Tela / Internet</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation>Não usar Perfís ICC lingados</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation>Exportar todas páginas para PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation>Exportar um intervalo de páginas para PDF </translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation>Período de tempo que a página é exibida antes que a 
apresentação comece na página selecionada.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation>Período em que o efeito acontece.
Quanto menor o tempo mais rápido será o efeito.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation>Aplicar os efeitos selecionados a todas as páginas.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation>Seleciona uma senha que habilita ou impede o 
uso de recursos de segurança em seu PDF exportado</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation>Adiciona um perfil de cor a uma cor sólida</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation>Adiciona um perfil de cor em imagens </translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation>Não usar perfís de cores que estejam adicionadas 
em sua fonte de imagens</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation>Distância para sangria do topo da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation>Distância para sangria da base da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation>Distância para sangria no lado esquerdo da página</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation>Distância para sangria no lado direito da página</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation>Criar Arquivo PDF</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>Nome do Arquivo de Sa&amp;ída:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>Alter&amp;ar...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation>&amp;Todas as Páginas</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation>Selec&amp;ionar Páginas</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation>Compat&amp;ibilidade:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation>&amp;União:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation>Gerar &amp;Visualização em Miniaturas</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation>Salvar Quadro de Texto &amp;Ligado como Artigo PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation>&amp;Incluir Favoritos</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Resolução:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation>&amp;Método:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Qualidade:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation>&amp;Alterar Resolução da Imagem para:</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation>&amp;Geral</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation>&amp;Incorporar todas as Fontes</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt;</translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt; </translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation>&amp;Fontes</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation>Mostrar &amp;Visualização de Página</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation>&amp;Tempo da Exibição:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation>Duração do Efei&amp;to:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation>Tipo de Efe&amp;ito:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation>&amp;Sentido:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation>&amp;De:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation>D&amp;ireção:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation>&amp;Aplicar Efeito em Todas as Páginas</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation>&amp;Usar Criptografia</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation>&amp;Usuário:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation>Pr&amp;oprietário:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation>Permitir Im&amp;pressão do Documento</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation>Permitir &amp;Alterações no Documento</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation>Permitir Cóp&amp;ia de Texto e Gráficos</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation>Permitir Adicionar &amp;Anotações e Campos</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation>S&amp;egurança</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation>Saída Pretendida para:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation>&amp;Usar Configurações Personalizadas de Renderizamento</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation>Configurações de Renderização </translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation>Fre&amp;quência:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation>&amp;Ângulo:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation>F&amp;unção Direcional:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation>Ponto Simples</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linha</translation>
    </message>
    <message>
        <source>Round</source>
        <translation>Ciclo</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation>Elipse</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation>Usar Perfil ICC</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation>C&amp;or</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation>&amp;Informação de Sequência:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation>Perfíl de Saída:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation>PDF/X-&amp;3 </translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Inserir uma lista separada por vírgula  de marcas que 
indicam aonde  notas podem ser encontradas. Use *para 
todas as páginas, 1-5 para um intervalo de páginas ou 
apenas um número, para uma página específica.</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation>Determinar a compatibilidade PDF. A padrão é Acrobat 
4.0 que dá a maior compatibilidade. 
Escolha Acrobat 5.0 se seu arquivo contém recursos de 
PDF 1.4 como transparência ou requer uma criptografia 
de 128 bit. 
PDF/X-3 é usado para exportação de PDF para impressão 
comercial e é selecionado quando você tem ativado o 
gerenciamento de cor.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation>Determina a união das páginas no PDF. Ao menos 
que você saiba que tem de mudar isso, deixe a opção 
padrão - esquerda.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation>Gerar visualização em miniatura de cada página 
no PDF. 
Alguns visualizadores podem usar a visualização 
em miniaturas para a navegação no PDF.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation>Gerar Artigos PDF, isto é útil na navegação de artigos 
ligados em um PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation>Incorporar favoritos ao PDF. 
Caso tenha criado um favoritos, o Scribus poderá o 
incorporar ao PDF. Isto é útil na navegação do documento.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation>Resolução para exportação de texto e vetores. 
Isto não tem efeito na resolução de imagens 
bitmap como fotos.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation>Compressão de textos e gráficos. 
Ao menos que você tenha alguma razão, deixe 
isso marcado. Isso reduz o tamanho do PDF.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation>Tipo de compressão para imagens. 
Automático permite que o Scribus selecione o 
melhor método. 
ZIP é bom para imagens com cores sólidas. 
JPEG é melhor na criação de pequenos arquivos PDF 
que contenham muitas fotos 
(com pequena perda de qualidade). 
Deixe em automático, ao menos que você precise de 
um tipo especial de opção de compressão.</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation>Níveis de compressão: Mínimo (25%), Baixo (50%), 
Médio (75%), Alto (85%), Máximo (95%)</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation>Redefinir suas imagens em bitmap para a resolução 
DPI selecionada. 
Deixando esse campo desmarcado, deixará as imagens 
serem renderizadas em sua resolução nativa.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation>DPI (Pontos por polegada) para imagens exportadas.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation>Incorporar fontes ao PDF. Isso fará com que a aparência 
do seu documento seja preservada.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation>Habilita feitos de apresentação quando usado o 
Acrobat Reader em tela cheia.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation>Mostrar visualização de página em cada página 
listada acima.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation>Tipo de efeitos de tela.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation>Direção do efeito de posição de linhas para divisão ou cortina.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation>Posição inicial dos efeitos de caixa e divisão.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation>Direção dos efeitos de brilho e sombra.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation>Habilita os recursos de segurança em seu PDF exportado. 
Se você selecionar Acrobat 4.0, o PDF será protegido por 
uma criptografia de apenas 40 bit. 
Se você selecionar Acrobat 5.0, o PDF será protegido por 
uma criptografia de 128 bit. 
Aviso: A criptografia do PDF não é tão confiável como a 
criptografia do GPG ou PGP e têm suas limitações.</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation>Escolha uma senha para que os usuários possam ler seu PDF.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation>Permitir impressão do PDF. Se desmarcado, a 
impressão é negada. </translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation>Permitir modificações no PDF. Se desmarcado, a 
modificação do PDF é negada.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation>Permitir cópia de texto ou gráficos do PDF. 
Se desmarcado, texto e gráficos não podem ser copiados.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation>Permitir adicionar campos e anotações ao PDF. 
Se desmarcado, a edição de anotações e campos é negada.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation>Modelo de cor para saída de seu PDF. 
Selecione Tela/Internet para PDF que são mostrados 
na tela e Impressão para típicas impressoras a jato de tinta. 
Escolha Impressora quando estiver imprimindo em uma 
verdadeira 4 cores CMYK.</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation>Perfíl de cor para cores sólidas</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation>Renderizamento pretendido para cores sólidas</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation>Perfil de Cor para imagens</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation>Renderizamento pretendido para imagens</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation>Prefíl de saída para Impressão. Se possível, obtenha 
informações de sua impressora na seleção de perfil.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation>A sequência mandatória para PDF/X-3 ou PDF irá falhar 
em adequação ao PDF/X-3. Nós recomendamos que você 
use o título do documento.</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>Arquivos PDF (*.pdf);;Todos os Arquivos (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation>Esta é uma opção avançada que não pode ser habilitada 
por padrão. Esta opção somente deve ser habilitada 
quando requisitada especialmente por sua gráfica e os 
detalhes necessários devem ser dados por eles. 
De outro modo, seu PDF exportado poderá não ser impresso 
 corretamente e isto é verdadeiro em sistemas não portáveis.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation>Compactar Texto e &amp;Vetores</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation>H&amp;abilitar Efeitos de Apresentação</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation>&amp;Apresentação</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotação:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation>&amp;Adicionar todas fontes ao sub-sistema</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation>Fontes para o sub-sistema:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation>Espelhar Página(s) Horizontalmente </translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation>Espelhar Página(s) Verticalmente</translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>All</source>
        <translation>Tudo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Visualizar Impressão</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Suavizar &amp;Texto</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Suavizar Gráficos</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Mostrar Trans&amp;parência</translation>
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
        <translation>Oferece um visualização mais agradável de ítens de 
texto, ao custo de redução de velocidade na exibição. 
Apenas afeta as fontes Tipo 1 (Type 1) </translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Mostra a transparência e ítens transparentes no seu documento. 
Requer GhostScript 7.07 ou superior</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Exibe uma visualização de impressão usando 
simulações de tintas CMYK genéricas, ao invés 
de cores RGB</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Habilitar/Desabilitar a tinta C (Ciano) na paleta </translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Habilitar/Desabilitar a tinta M (Magenta)  na paleta </translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Habilitar/Desabilitar a tinta Y (Amarelo) na paleta </translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Habilitar/Desabilitar a tinta K (Preto) na paleta </translation>
    </message>
    <message>
        <source>&amp;Under Color Removal</source>
        <translation>Remover &amp;Sobre Cor</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>O UCR desabilita sombras que são compostas por 
ciano, amarelo e magenta é utiliza preto no lugar. 
No UCR a maior parte das cores afetadas são imagens 
que são neutras e/ou de tons escuros que são compostos 
por cinza. A utilização desse modo melhora a impressão 
de algumas imagens e alguns experimentos e testes são 
necessários de caso para caso. O UCR reduz a 
possibilidade de saturação com as cores CMY.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation>Oferece uma exibição mais agradável de fontes 
TrueType, OpenType, EPS, PDF, e vetores gráficos 
na visualização, ao custo de uma pequena redução 
de velocidade. </translation>
    </message>
</context>
<context>
    <name>Page</name>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation>Quadro de texto</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation>Copiar Aqui</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation>Mover Aqui</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation>Editar Texto...</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation>Imagem</translation>
    </message>
    <message>
        <source>File: </source>
        <translation>Arquivo:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation>Texto linkado</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation>Texto ao Caminho</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Parágrafos:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palavras:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Carácteres:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation>Imprimir:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation>Habilitado</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation>Desabilitado</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation>O Programa</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation>está faltando!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation>Copiar de</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Colar</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Mostrar &amp;Margens</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Mostrar &amp;Quadros</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Mostrar &amp;Imagens</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Mostrar &amp;Grelha</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Mostrar G&amp;uias</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Mostrar Linha &amp;Base da Grelha</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Gruda&amp;r a Grelha</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Gruda&amp;r a Guias</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation>PPI de Origem: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation>PPI Atual: </translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation>In&amp;formações</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation>&amp;Obter Imagem...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation>I&amp;magem Visível </translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation>&amp;Atualizar Imagem</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation>&amp;Editar Imagem</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation>&amp;Ajustar Quadro a Imagem</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Obter Texto...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation>&amp;Anexar Texto...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation>&amp;Editar Texto...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation>&amp;Inserir Texto de Exemplo</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation>É um &amp;Favorito de PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation>É uma A&amp;notação de PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation>P&amp;ropriedades de Anotação</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation>P&amp;ropriedades de Campo</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation>Opções de &amp;PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Desbloquear</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation>Bloquear Tamanho do Objeto</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation>Desbloquear Tamanho do &amp;Objeto</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation>Enviar para Galeria</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation>Enviar para Cam&amp;ada</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>&amp;Agrupar</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation>Desa&amp;grupar</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation>Ní&amp;vel</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Enviar para &amp;Trás</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Trazer à &amp;Frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Abaixo</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Acima</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation>&amp;Quadro de Imagem</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation>Pol&amp;ígono</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation>&amp;Contornos</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation>Quadro de &amp;Texto</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation>Curva de &amp;Bezier</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation>Conve&amp;rter em</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation>&amp;Limpar Conteúdo</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation>Mostrar P&amp;ropriedades...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation>Ocultar P&amp;ropriedades...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Você quer realmente apagar todo o seu texto?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Imagem</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linha</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polyline</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Caminho de Texto</translation>
    </message>
</context>
<context>
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation>Página</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation>de %1</translation>
    </message>
</context>
<context>
    <name>PicSearch</name>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Result</source>
        <translation>Resultado</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Selecionar</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Pesquisar Resultados para: </translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Visualizar</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Ir para</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Caminho</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Imprimir</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Faltando</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Pesquisar</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Estado</translation>
    </message>
    <message>
        <source>Pictures</source>
        <translation>Imagens</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
</context>
<context>
    <name>PolygonProps</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source>Polygon Properties</source>
        <translation>Propriedades de Polígono</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Ca&amp;ntos:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotação:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Fator</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Número de cantos dos polígonos</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grau de rotação dos polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Exemplo do Polígono</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplicar &amp;Fator</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplicar Fator Convexo/Côncavo para mudar a forma do polígono</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Um valor negativo fará com que o polígono 
fique côncavo, um valor positivo fará com que 
ele fique convexo. </translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source> %</source>
        <translation> %</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source> in</source>
        <translation>pol</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Interface Gráfica</translation>
    </message>
    <message>
        <source>min</source>
        <translation>min</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menus</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Caminhos</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Outro</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Pequeno</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Units</source>
        <translation>Unidades</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Outras Opções</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Letras Minúsculas</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Guias</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation>Cores de Grelha</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Médio</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation>Aparência da Grelha</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Tipografia</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation>Posicionamento</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guia de Margens</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Visualizar</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation>Pijamas de seda trocados para quartzo azul</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Galeria</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation>Para ajustar a exibição, arraste a régua abaixo com o controle.</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation>Picas (p)</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscrito</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Sobrescrito</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation>Pontos (pt)</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Exibir</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolher um Diretório</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation>Polegadas (pol)</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Salvar Automaticamente</translation>
    </message>
    <message>
        <source>Preferences</source>
        <translation>Preferências</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Geral</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Ferramentas Externas</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation>Outros</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation>Milímetros (mm)</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation>Interpretador Postscript</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Ferramenta de Processamento de Imagem</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Impressão</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Selecione um tema como padrão.
Scribus aceita qualquer tema de KDE e QT disponível</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Tamanho da fonte padrão para menus e janela</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Unidade de medida padrão para edição do documento</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Número de linhas deslocadas pelo programa a cada 
movimento da roda do mouse</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation>Área do raio aonde o gerenciador de objetos pode ser selecionado</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Número de documentos recentes a serem mostrados 
no menu Arquivo</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Diretório padrão de documentos</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation>Diretório padrão de Perfil ICC</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Diretório padrão de scripts</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Tamanho Padrão de Página, comum ou personalizado</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Orientação Padrão das Páginas do Documento</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Largura das Páginas do Documento, editável se 
você escolheu uma página personalizada</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Altura da Página no documento, editável se 
você escolheu uma página personalizada</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Habilitar visão simples ou extendida</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation>Faz a primeira página à esquerda do documento</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distância entre o topo da margem e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distância entre a base da margem e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem esquerda e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem direita e a borda da página</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Quando habilitado, o Scribus salva uma ćopia 
do seu arquivo com a extensão .bak
cada vez que se passar um período definido</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Período de tempo para salvar automaticamente</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation>Distância entre as menores linhas da Grelha</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation>Distância entre as maiores linhas da Grelha</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation>Distância que um objeto se posicionará da guia 
inserida no documento</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation>Cor da menor linha de Grelha</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation>Cor da maior linha de Grelha</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation>Cor da linha guia inserida</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation>Posicionar a grelha atrás dos objetos de sua página </translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation>Posicionar a grelha a frente dos objetos de sua página </translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation>Deslocar acima da linha base de uma fonte 
em uma linha </translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation>Tamanho relativo do sobrescrito comparado 
a uma fonte normal </translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation>Deslocar abaixo da linha base de 
uma fonte em uma linha </translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation>Tamanho relativo do subscrito comparado a 
uma fonte normal </translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation>Tamanho relativo das minúsculas comparado a 
uma fonte normal </translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation>Porcentagem de ampliação sobre o tamanho 
da fonte para o entrelinhas</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation>Propriedades do  Quadro de Texto</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation>Propriedades do Quadro de Imagem</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation>Propriedades de Desenho de Forma</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation>Nível de Magnificação Padrão</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation>Propriedades de Desenho de Linha</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation>Propriedades de Desenho de Polígonos</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation>Fonte para novos quadros de texto</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation>Tamanho da fonte para novos quadros de texto</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation>Cor da Fonte</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation>Número de Colunas em um quadro de Texto</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation>Espaço entre colunas no quadro de texto </translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation>Visualização da Fonte</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation>Quadros de imagens permitem que as imagens 
sejam redimensionadas a qualquer tamanho</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation>Proporção Horizontal das imagens</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation>Proporção Vertical das imagens</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation>Manter proporção horizontal e vertical proporcionais</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation>Imagens em um quadro de imagems são dimensionadas 
no mesmo tamanho do quadro</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation>Imagens dimensionadas mantêm suas proporções 
originais automaticamente</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation>Cor de preenchimento dos quadros de imagem</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation>Saturação da cor de preenchimento</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation>Cor da linha das formas</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation>Saturação da cor das linhas</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation>Cor de preenchimento das formas</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation>Estilo de linha das formas</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation>Espessura da linha das formas</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation>Magnificação mínima disponível</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation>Magnificação máxima disponível</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation>Alterar magnificação a cada operação de zoom</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation>Cor das linhas</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Saturação da cor</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation>Estilo de linha </translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation>Espessura de linhas</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation>Número de cantos dos polígonos</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation>Grau de rotação dos polígonos</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation>Polígono de Exemplo</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Selecionar o tamanho da visualização na paleta da Galeria</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation>Mostrar páginas lado a lado quando usar páginas de rosto</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Cor para o papel</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation>Cor para as linhas da margem
</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Mascarar a área fora das margens na cor de margens</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation>Habilitar recurso de tranparência dentro da exportação PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Definir nível padrão do zoom</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation>Local dos arquivos do sistema para o interpretador Ghostscript</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Suavizar Texto EPS e PDF para renderização de tela</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Suavizar gráficos EPS e PDF para renderização de tela</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Não mostrar objetos fora das margens numa 
página impressa ou arquivo exportado</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Tema:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>Tamanho da &amp;Fonte:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation>&amp;Configurações do Mouse</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Percurso da roda:</translation>
    </message>
    <message>
        <source> px</source>
        <translation> px </translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation>&amp;Tomada do Raio:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>Documentos &amp;Recentes:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>&amp;Documentos:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Alterar...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>Perfis &amp;ICC:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>Alt&amp;erar...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Scripts:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>Alter&amp;ar...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamanho:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>Orie&amp;ntação:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Largura:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Altura:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Páginas de Rosto</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Primeira P&amp;ágina à Esquerda</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Fundo:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Topo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Direita:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Esquerda:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>&amp;Habilitado</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Intervalo:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation>M&amp;enor Espaçamento da Grelha:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation>Ma&amp;ior Espaçamento da Grelha:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation>Guia Mantém a Distância:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation>Cor da Grelha Men&amp;or:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation>Cor da Grelha Maio&amp;r:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation>Cor das Guias do &amp;Usuário:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation>Cor da &amp;Linha Base da Grelha:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation>No &amp;Plano de Fundo</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation>No &amp;Plano Frontal</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation>Linha Base da Grelha</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation>Ligad&amp;o</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation>Desligad&amp;o</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation>&amp;Deslocamento:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation>&amp;Proporção:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation>De&amp;slocamento:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation>Prop&amp;orção:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation>Prop&amp;orção:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation>Linha Base da &amp;Grelha:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation>Intervalo da Linha &amp;Base:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation>Espaçamento Automático de &amp;Linha:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation>&amp;Fonte Padrão:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation>Tamanho &amp;Padrão:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation>Cor de &amp;Texto:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>Colu&amp;nas:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Distância:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation>Cor da &amp;Linha:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation>&amp;Sombreamento:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation>Cor de &amp;Preenchimento:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation>&amp;Sombreamento:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation>&amp;Tipo de Linha:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>&amp;Espessura de Linha:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation>Mí&amp;nimo:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation>Má&amp;ximo:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation>&amp;Passo a passo:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Redimensionamento Livre</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation>Proporção &amp;Horizontal:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation>Proporção &amp;Vertical:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation>&amp;Ajustar Imagem ao Tamanho do Quadro</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation>Manter P&amp;roporção</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation>Cor de &amp;Preenchimento:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation>Cant&amp;os:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Rotação:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation>&amp;Fator:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>Sal&amp;var Conteúdo após Alterações</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Grande</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation>Mostrar Páginas &amp;Lado a Lado</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation>Cor da Página</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation>&amp;Plano de Fundo:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation>&amp;Margens:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Mostrar Área &amp;não Imprimível com a cor da Margem</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation>Usar  Recursos de &amp;Transparência de PDF 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>&amp;Ajustar Tamanho de Vídeo</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>&amp;Nome do Executável:</translation>
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
        <source>Name of &amp;Executable:</source>
        <translation>Nome do Executável:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Grudar às &amp;Margens</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation>Ligar as linhas base da grelha</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation>Desligar as linhas base da grelha</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Toda vez salvar o conteúdo da Galeria 
depois de uma alteração</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation>Localização no sistema de arquivos para o editor gráfico</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>&amp;Interno:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation>E&amp;xterno:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Aplicar Remoção de S&amp;obre Cor (UCR)</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation>&amp;Modelos:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>A&amp;lterar...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation>Aplicar Fator</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation>Diretório Adicional para Documentos Modelos</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation>Aplicar Fator Convexo/Côncavo para mudar a 
forma dos polígonos</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation>Um valor negativo fará com que o polígono fique 
côncavo, um valor positivo fará com que ele fique convexo </translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>O UCR desabilita sombras que são compostas por 
ciano, amarelo e magenta é utiliza preto no lugar. 
No UCR a maior parte das cores afetadas são imagens 
que são neutras e/ou de tons escuros que são compostos 
por cinza. A utilização desse modo melhora a impressão 
de algumas imagens e alguns experimentos e testes são 
necessários de caso para caso. O UCR reduz a 
possibilidade de saturação com as cores CMY.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation>Executivo</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation>Fólio</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation>Base</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal </translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tablóide </translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>Hu&amp;e:</translation>
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
        <translation>Ve&amp;rmelho:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>V&amp;erde:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>Az&amp;ul:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>Canal A&amp;lpha:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Cores básicas</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Cores Personalizadas</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Definir cores Personalizadas &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Adicionar a Cores Personalizadas </translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Selecionar Cor</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Copiar ou mover um arquivo</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Ler: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Escrever: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>&amp;Nome do arquivo:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>&amp;Tipo de arquivo:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>Um diretório acima</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamanho</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Locali&amp;zação:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Voltar</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Criar Nova Pasta</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Visualizar em Lista</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Visualizar em detalhes</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Visualizar informação de arquivo</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Visualizar Conteúdo de Arquivo</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Leitura-Escrita</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Somente Leitura</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Somente Escrita</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Inacessível</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Symlink para Arquivo</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Symlink para Diretório</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Symlink para Especial</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Diretório</translation>
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
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Renomear</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>R&amp;ecarregar</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Organizar por &amp;Nome</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Organizar por &amp;Tamanho</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Organizar por &amp;Data</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Aleatório</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Organizar</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Exibir arquivos &amp;ocultos</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>o arquivo</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>o diretório</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>o symlink</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Apagar %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Você realmente deseja apagar %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Sim</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Não</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Nova Pasta 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Nova Pasta</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Nova Pasta %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Encontrar Diretório</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Diretórios</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Erro</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Arquivo não encontrado.
Cheque o caminho e o nome do arquivo.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Todos Arquivos (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Selecione um diretório</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Diretório:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Fonte</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Estilo de Fonte</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamanho</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Efeitos</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Tachado </translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>S&amp;ublinhado</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Cor</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Exemplo</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>Sc&amp;ript</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
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
        <translation>Fechar</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Selecionar Fonte</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Colar</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Alinhar</translation>
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
        <translation>&lt;h3&gt;Sobre Qt&lt;/h3&gt;&lt;p&gt;Esse programa usa Qt versão %1.&lt;/p&gt;&lt;p&gt;Qt é um utilitário C++ multiplataforma GUI &amp;amp; aplicativos de desenvolvimento.&lt;/p&gt;&lt;p&gt;Qt suporta MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, e a maioria de variantes Unix.&lt;br&gt;Qt também é disponível para dispositivos acoplados.&lt;/p&gt;&lt;p&gt;Qt é um produto Trolltech. Acesse &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; para mais informações.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>No</source>
        <translation>Não</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Você deseja realmente substituir o Arquivo:
%1 ?</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar Como</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Referência Online</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Initializing...</source>
        <translation>Inicializando...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Plano de Fundo</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation>Visualizar Impressão</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation>S&amp;cript</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation>Python Scripts (*.py);; Todos os Arquivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>Imagens-SVG (*.svg *.svgz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>Imagens-SVG (*.svg);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation>Visualizar &amp;Fontes</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation>&amp;Inserir Especial</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation>Novo a partir do Modelo...</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Newsletters</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Brochuras</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Catálogos</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Flyers</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Assinaturas</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Cartões</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Cabeçalho</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Envelopes</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Cartão de Negócios</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Calendários</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Anúncios</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Rótulos</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Menus</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Programas</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>Formulários PDF</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>Apresentações PDF</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Revistas</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Cartaz</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Anúncios</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Documentos de Texto</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Dobras</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Modelos Próprios</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation>Salvar como &amp;Imagem...</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Salvar Imagem como</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation>Erro ao escrever o(s) arquivo(s) de saída.</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Erro ao ler o(s) arquivo(s) de saída.</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Exportação concluída.</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Arquivo existente. Sobrescrever?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>já existente. Sobrescrever?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Sim para todos</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation>Visualizar Impressão</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation>Importar &amp;EPS/PS...</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Todos os Formatos suportados (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation>Salvar como Modelo...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation>Manual do S&amp;cripter...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation>Scripts do &amp;Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation>&amp;Executar Script...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation>Scripts &amp;Recentes</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation>Mostrar &amp;Terminal</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation>Salvar Página como &amp;SVG...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation>Importando &amp;SVG...</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Importanto texto</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos formatos Suportados</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>Arquivos HTML</translation>
    </message>
    <message>
        <source>html</source>
        <translation>Html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Arquivos de textos</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Arquivos com valores separados por vírgula</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>Data CSV</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>Leitor de CSV</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation>Modelos. </translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>
Ligações Externas</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>A fonte %1 está quebrada, discartando-a</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Filtros de Textos</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Casos de Mídia</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Albanês</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Basco</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Búlgaro</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Brasileiro</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalão</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Chinês</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tcheco</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dinamarquês</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandês</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglês</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Inglês (Britânico)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Esperanto</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemão</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandês</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francês</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Galão</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grego</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Indonésio</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Koreano</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Norueguês (Bokmaal)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Norueguês (Nnyorsk)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Norueguês</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polonês</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russo</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Sueco</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Espanhol</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Espanhol (Latino)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Sloveno</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Sérvio</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation>Tentando definir progresso &gt; progresso máximo</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation>Sobre o Script...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation>Sobre o Script</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter o tamanho da fonte em um 
 quadro que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter uma fonte em um quadro 
que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter o tamanho do texto em um 
quadro que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível contar as colunas em um quadro 
que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter o entrelinhas em um quadro 
 que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter a lacuna de coluna em um 
quadro que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível obter um texto em um quadro 
que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível definir um texto em um quadro 
que não seja de texto.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Não é possível inserir um texto em um quadro 
que não seja de texto.</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation>Inserção de índice fora do limite</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Valor de alinhamento fora do limite. Use um dos 
valores de alinhamento do Scribus.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir o alinhamento de texto em um 
quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation>Tamanho da fonte fora do limite, deve estar 
entre 1 &lt; = Tamanho &lt;= 512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir o tamanho da fonte em um 
quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir uma fonte em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation>Fonte não encontrada</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation>Entrelinhas fora do limite, deve ser &gt; = 0.1</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível espaçar um linha em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation>Lacuna de coluna fora do limite, deve ser positivo</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir uma lacuna de coluna em 
um quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation>Contagem de colunas fora do limite, deve ser &gt; 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível numerar uma coluna em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Seleção de índice fora do limite</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível selecionar um texto em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível apagar um texto em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir o preenchimento de texto 
em um quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir uma linha em um quadro 
que não seja de texto</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível definir sombra de texto em um 
quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation>Só é possível ligar quadros de texto</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation>O quadro alvo deve estar vazio</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation>O quadro alvo liga a outro quadro</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation>O quadro alvo está ligado a outro quadro</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation>Fonte e alvo são os mesmos objetos</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível desligar um quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation>O objeto não está ligado ao quadro de texto, não 
é possível desligar.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation>Objeto do último quadro em uma série não pode 
ser desligado. Desligue o quadro anterior no lugar.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation>Não é possível converter um quadro que não seja 
de texto para contornos</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation>Falha em abrir o documento</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation>Falha em salvar o documento</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Unidade fora do limite. Use uma das unidadas do Scribus.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>O alvo não é um quadro de imagem.</translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation>Não é possível proporçãor em 0%</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation>Não é possível renderizar um exemplo vazio</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation>Não é possível salvar comum nome em branco</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation>Não é possível haver camadas sem nome</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation>Camada não encontrada</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation>Não é possível remover a última camada</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation>Não é possível criar uma camada sem um nome</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation>Um objeto com o nome requisitado já existe</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation>Ponto de lista deve conter ao menos dois 
pontos (quatro valores)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation>Ponto de lista deve conter um mesmo 
número de valores</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation>Ponto de lista deve conter ao menos 
três pontos (seis valores)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation>Ponto de lista deve conter ao menos 
quatro pontos (oito valores)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation>Ponto de lista deve ter um multiplicador 
de seis valores</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation>Objeto não encontrado</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation>Estilo não encontrado</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation>Não é possível colocar um estilo em um 
quadro que não seja de texto</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation>Falha em salvar EPS</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation>Número de página fora do limite</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation>argumento não listado: deve ser listado 
em valores flutuantes </translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation>argumento contém valores não numéricos: </translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation>Espessura da Linha fora do limite, deve estar 
entre 0 &lt; = espessura da linha &lt; = 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Linha da sombra fora do limite, deve estar 
entre 0 &lt; = Sombra &lt; = 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation>Preencher sombra fora do limite, deve estar 
entre 0 &lt; = Sombra &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>O canto em radiando deve ser um número positivo.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation>Estilo de linha não encontrado</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Não é possível obter uma cor sem um nome.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation>Cor não encontrada</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Não é possível mudar uma cor sem um nome.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation>Cor não encontrada no documento</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation>Cor não encontrada em cores padrões</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Não é possível criar uma cor com se um nome.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Não é possível apagar um cor sem um nome.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Não é possível substituir uma cor sem um nome.</translation>
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
        <translation type="obsolete">Módulo de interface Python do Scribus 
Esse é o módulo de interface Python para o Scribus. Ele oferece funções de controle do Scribus e de manipulação de objetos na tela. Cada função é documentada individualmente logo abaixo. 
Poucas coisa são comuns na maioria da interface. 
A maioria das funções é operada em quadros. Quadros são identificados por seus nomes, um valor - eles não são realmente objetos Python. Muitas funções levam um parâmetro opcional (não uma palavra-chave), um nome de um quadro. 
Muitas excessões são comuns na maioria das funções. Essas excessões não estão atualmente gravadas no docstring de cada função. 
.... - Muitas funções abrirão a janela de NoDocOpenError se você tentar usa-las sem um documento para operar. 
.... - Se você não der um nome de um quadro para uma função que necessite de um, a função usará o quadro atualmente selecionado. Caso nenhum quadro estiver selecionado, uma janela de NoValidObjectError poderá aparecer indicando que não foi encontrado nada para ser operado pela função. 
.... - Muitas das funções abrirão a janela WrongFrameTypeError se você tentar usa-las em um quadro de texto que não faça sentido com a função. Por exemplo, definindo a cor de seu texto em um quadro gráfico não faz sentido, e resultará na abertura da janela de erro. 
.... - Erros resultados de chamadas para a biblioteca Python de sublinhamento, passarão sem sofrer modificações. Como tal, a lista de excessões surge em qualquer função citada aqui, mesmo estando esta lista incompleta. 
Detalhes de quais excessões  de cada função podem aparecer ou são providas na documentação das funções. 
</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation>Importar do &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Todos os Arquivos (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Documentos do OpenOffice.org Writer </translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation type="unfinished">Africano</translation>
    </message>
    <message>
        <source>Portugese (Brazilian)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Specified item not an image frame</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
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
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Colar</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Menu do sistema</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Remover Sombra</translation>
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
        <translation>Fechar</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Refazer</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Mover</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamanho</translation>
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
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>Ficar no &amp;Topo</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Minimizar</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Refazer Abaixo</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fechar</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>So&amp;mbra</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>&amp;Remover Sombra</translation>
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
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Configurações do Documento</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Guia de Margens</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Topo:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Esquerda:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Fundo:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Direita:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation>&amp;Página de Rosto</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation>Primeira &amp;Página à Esquerda</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation>Habilitar visão simples ou extendida</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation>Fazer a primeira página a página esquerda do documento</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation>Distância entre o topo da margem e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation>Distância entre a base da margem e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem esquerda e a borda da página</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation>Distância entre a margem direita e a borda da página. 
</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation>Dentro:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation>F&amp;ora:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation>Tamanho:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Personalizado</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation>Orientação:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Retrato</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Paisagem</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Largura:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation>Altura:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Número da Primeira Página:</translation>
    </message>
</context>
<context>
    <name>SToolBAlign</name>
    <message>
        <source>Style of current paragraph</source>
        <translation>Estilo do parágrafo atual</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Configurações de Estilo</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>nenhum</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Cor de preenchimento do texto</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Saturação da cor do preenchimento de texto</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Configurações de preenchimento de Cor</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>nenhum</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Cor da borda do texto</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Saturação da cor da borda do texto</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Configurações da Cor de Linha</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
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
        <translation>Fonte do texto selecionado</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da fonte</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Dimensão da largura dos caracteres</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Configurações de Fonte</translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation>Kerning:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation>Manual Kerning</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Configurações de Caractere</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Background</source>
        <translation>Plano de Fundo</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation>Copiar #%1 de </translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>OK</source>
        <translation>OK</translation>
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
        <source>All</source>
        <translation>Todo</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Novo</translation>
    </message>
    <message>
        <source>200%</source>
        <translation>200%</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nenhum</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation>Sair</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Salvar</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Tamanho</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation>Saindo Agora</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation>Desfazer</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation>Alinhamento á Direita</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation>Tachado</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation>Alinhar ao Centro</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation>Estilos...</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation>Distribuir/Alinhar...</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation>Limpar</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Fechar</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Cor</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Tcheco</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Holandês</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Grego</translation>
    </message>
    <message>
        <source>Group</source>
        <translation>Grupo</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation>Abaixar</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation>Colar</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation>Subir</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Pronto</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>Style</source>
        <translation>Estilo</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation>Enviar para trás</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation>Os Programas seguintes estão faltando:</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Italiano</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation>Grudar à Grelha</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Inicializando Plugins</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation>Desagrupar</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation>Hífenização Inteligente</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation>EPS-Files (*.eps);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation>Apagar...</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Minúsculas</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Duplicar Múltiplicar</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation>Mostrar Paleta da Página</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation>Cores...</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation>Visualização em miniaturas</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Pesquisando Fontes</translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation>Informação de Documento...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation>Mover...</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation>Ocultar Quadros</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Dinamarquês</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Apagar</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation>Sem quebra de espaço</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation>Ocultar Imagens</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Húngaro</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Francês</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Alemão</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation>Bloquear/Desbloquear</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Inverter</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation>Não há Fontes Postscript no seu Sistema</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation>Mostrar Camadas</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Polonês</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation>Carregando:</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*) </translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Eslovaco</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Documento</translation>
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
        <source>Unlock</source>
        <translation>Desbloquear</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation>Fontes...</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation>Abrir...</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation>Mostrar Grelha</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation>Duplicar</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation>Ocultar Margens</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation>Salvar como...</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation>Trazer Imagem...</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation>Anexar Texto ao Caminho</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation>Inserir Número de Página</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation>Trazer á Frente</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation>Ajustando Cores</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation>Gerenciar Guias...</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Ukraniano</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Lendo Galeria</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation>Configurações do Documento...</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation>Javascripts...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation>Ajustar a Janela</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation>Imprimir...</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Sobre Qt</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation>Trazer Texto/Imagem...</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Catalão</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation>Aplicar Modelo...</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscrito</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Sobrescrito</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation>Inserir...</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Russo</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Falha na impressão!</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation>Hifenar Texto</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd .scd.gz);; Todos Arquivos(*) </translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Espanhol</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation>Obter Texto...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Imprimindo...</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Sublinhar</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Carregando...</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Escolha um diretório</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation>Ajuda Online...</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Português</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation>Gerenciar Imagens</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Inglês</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Delineado</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation>Modelos...</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Salvando...</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Lendo Preferências</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Configurando teclas de atalho</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Erro Fatal</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation>Alinhamento a Esquerda</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation>Selecionar Nova Fonte</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation>Sobre Scribus</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation>Inicializar Hifenador</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation>Não pode escrever o Arquivo: 
%1</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Manual do Scribus</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation>Criar um novo Documento</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation>Abrir um Documento</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation>Salvar o Documento Atual</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation>Fechar o Documento Atual</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation>Imprimir o Documento Atual</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation>Salvar Documento Atual como PDF</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Propriedades</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Contorno</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Galeria</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Objeto</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>&amp;Página</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Visualizar</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Ferramentas</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>E&amp;xtras</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>&amp;Janelas</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>&amp;Ajuda</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Todos Formatos Suportados</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Arquivos de Texto (*.txt);;Todos Arquivos(*)</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Finlandês</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Irlandês</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation>Erro no Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation>Scribus travou devido ao Sinal #%1</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation>&amp;Gerenciamento de Cor...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Abrir...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Abrir &amp;Recente</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Salv&amp;ar Como...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation>Rev&amp;erter para Salvo</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation>Colecionar para Saída...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation>Obter Texto/Imagem...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation>Anexar ao &amp;Texto...</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation>Importar &amp;Páginas...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation>Salvar &amp;Texto...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation>Salvar Página como &amp;EPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation>Salvar como P&amp;DF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Exportar</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation>Configurações do Documento...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation>Im&amp;primir...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation>Sair</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Desfazer</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Colar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Selecionar Tudo</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Procurar/Substituir...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation>C&amp;ores...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation>Estilo de &amp;Parágrafo...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation>&amp;Estilo de Linha...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation>&amp;Modelos...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation>&amp;Javascripts...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation>Duplicação &amp;Múltipla</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation>A&amp;grupar</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation>Desagr&amp;upar</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation>Bloquear</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation>Enviar para &amp;Baixo</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation>Trazer para &amp;Frente</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation>&amp;Abaixo</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation>&amp;Acima</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation>Distribuir/&amp;Alinhar...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>Forma</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation>&amp;Anexar Texto ao Path</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation>&amp;Desanexar Texto ao Path</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation>&amp;Unir Polígonos</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation>Separar &amp;Polígonos</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation>C&amp;onverter para Contornos</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation>&amp;Inserir...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation>&amp;Apagar...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation>&amp;Mover...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation>&amp;Aplicar Modelo...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation>Ajustar à janela</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation>&amp;100%</translation>
    </message>
    <message>
        <source>100%</source>
        <translation>100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation>Visualizar em minia&amp;tura</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation>Mostrar Margens</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation>Mostrar Quadros</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation>Mostrar Imagens</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation>Mostrar Grelha</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation>Mostrar Guias</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation>Mostrar Linha de Base da Grelha</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation>Grudar a Grelha</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation>Grudar as Guias</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation>&amp;Propriedades</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation>&amp;Esquema</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation>&amp;Galeria</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation>&amp;Camadas</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation>Organizar P&amp;áginas</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation>&amp;Favoritos</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation>&amp;Gerenciar Imagens</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation>&amp;Hifenizar Texto </translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation>Di&amp;cas de Ferramentas</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation>Ferramentas de P&amp;DF</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation>Dicas de Ferramentas</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation>P&amp;referências...</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation>&amp;Fontes...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation>&amp;Hifenador...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation>&amp;Teclas de Atalho...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation>&amp;Sobre o Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation>Sobre o &amp;QT</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation>&amp;Manual do Scribus...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>E&amp;stilo</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Configurações</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation>&amp;Esquerda</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation>&amp;Centro</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation>Di&amp;reita</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation>&amp;Bloquear</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation>&amp;Forçar</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation>&amp;Outro...</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation>&amp;Cascata</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation>&amp;Ajustar à Janela</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Cores</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation>&amp;Inverter</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation>&amp;Obter Texto...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Fonte</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Tamanho</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Efeitos</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>&amp;Alinhamento</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Sombra</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation>&amp;Tabuladores...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation>Desbloq&amp;uear</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Importando Páginas...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Importar Página(s)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Você está tentando importar mais páginas 
do que é possível no documento atual contando da 
página ativa.&lt;/p&gt; scolha um dos seguintes:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Criar&lt;/b&gt;páginas perdidas&lt;/li&gt;&lt;li&gt;&lt;b&gt;Importar&lt;/b&gt;páginas até a última página&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancelar&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Criar</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Imporar</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Importação concluída</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Nada foi encontrado para importar</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation>Ocultar Linha de Base da Grelha</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation>Mostrar Linha de base da Grelha</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation>Ghostscript : Você não pode usar Imagens EPS</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation>Alguns objetos estão bloqueados.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation>Desbloquear todos</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation>Bloquear todos</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Lituano</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Suíço</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Sloveno</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation>Obter perfil ICC</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation>Gerenciar &amp;Guias...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Tamanho:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Sombra:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation>&amp;Informações do Documento...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation>Desfa&amp;zer Apagar Objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation>Desfa&amp;zer Movimentar Objeto</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation>Desfa&amp;zer Mudança no Objeto</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation>&amp;Editar Forma</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation>O Arquivo %1 não é um formato Scribus</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Africano</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Fontes do Sistema Inicializadas</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todo</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>Camada</translation>
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
        <translation>pol</translation>
    </message>
    <message>
        <source>p</source>
        <translation>picas</translation>
    </message>
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>has been changed since the last save.</source>
        <translation>foi alterado desde a última vez que foi salvo.</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>O documento:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>&amp;Sair sem Salvar</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>&amp;Fechar sem Salvar</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>&amp;Salvar Agora</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Script File:</source>
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
    <message>
        <source>Close without saving changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="unfinished">Ooções Avançadas</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Mostrar Visualização de Página</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation>Mostrar Nome de Modelos</translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Pesquisar/Substituir</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Pesquisar por:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Estilo do Parágrafo</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Fonte</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Tamanho da Fonte</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Efeitos de Fonte</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Cor de Preenchimento</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Preencher Sombra</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Cor do Contorno</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Sombra do Contorno</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Esquerda</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Direita</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Bloco</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Forçado</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Nada</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Substituir com:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Pesquisa Terminada</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>Palavra Toda</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ignorar Maiúsculas</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>Procurar</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>Substitui&amp;r</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Substituir Tod&amp;as</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Normal</source>
        <translation>Normal</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation>Modelos Disponíveis:</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation>Página Esquerda primeiro</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Páginas de Rosto</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Páginas do Documento:</translation>
    </message>
    <message>
        <source>Arrange Pages</source>
        <translation>Organizar Páginas</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation>Arrastar Páginas ou Páginas Modelos para a 
Lata de Lixo para apaga-lás.
</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation>Aqui estão todos os seus Modelos, para criar uma 
Página nova arraste o Modelo para a Visualização 
de Página abaixo.</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation>Visualizar todas as páginas do seu documento.</translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Available Fields</source>
        <translation>Campos Disponíveis</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Campos Selecionados</translation>
    </message>
    <message>
        <source>Select Fields</source>
        <translation>Selecionar Campos</translation>
    </message>
    <message>
        <source>&amp;&gt;&gt;</source>
        <translation>&amp;&gt;&gt; </translation>
    </message>
    <message>
        <source>&amp;&lt;&lt;</source>
        <translation>&amp;&lt;&lt; </translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Outro...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Sombra</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Sombra:</translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Sem Estilo</translation>
    </message>
</context>
<context>
    <name>Spalette</name>
    <message>
        <source>No Style</source>
        <translation>Sem Estilo</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>No</source>
        <translation>Não</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Sim</translation>
    </message>
    <message>
        <source>Edit Styles</source>
        <translation>Editar Estilos</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Novo Estilo</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Copiar de %1</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Documentos (*.sla *.sla.gz *.scd *.scd.gz);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Documentos (*.sla *.scd);;Todos Arquivos (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation>&amp;Anexar</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>D&amp;uplicar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Salvar</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Você realmente deseja apagar esse estilo?</translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>File</source>
        <translation>Arquivo</translation>
    </message>
    <message>
        <source>Story Editor</source>
        <translation>Editor de Texto</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation>Você quer realmente apagar todo o seu texto?</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Atualizar Quadro de Texto</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Parágrafo Atual:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Palavras:
</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Caracteres:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Total:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Parágrafos:</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation>Você realmente deseja perder todas as alterações?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Abrir</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Arquivos de Texto (*.txt);;Todos Arquivos(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Salvar como</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Você deseja salvar suas alterações?</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Novo</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Recarregar Texto do Quadro</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>&amp;Salvar para arquivo...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>Carregar de arquivo...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Salvar &amp;Documento</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>At&amp;ualizar o Quadro de Texto e Sair</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Sair sem Atualizar o Quadro de Texto</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Corta&amp;r</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Copiar</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>Colar</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation>&amp;Inserir Especial...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>At&amp;ualizar Quadro de Texto</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Arquivo</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Selecion&amp;ar Tudo</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Editar Estilos...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Carregar Texto de um Arquivo</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Salvar Texto para Arquivo</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Atualizar Quadro de Texto e Sair</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Sair sem Atualizar o Quadro de Texto</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Recarregar Texto do Quadro</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>Procurar/&amp;Substituir...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>Visualizar &amp;Fontes ...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>Plano de Fundo...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>Exibir Fonte...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>Configurações</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Pesquisar/Substituir</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation>Limpar todo Texto</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Seleção Inteligente de Texto</translation>
    </message>
</context>
<context>
    <name>StyleSelect</name>
    <message>
        <source>Underline</source>
        <translation>Sublinhado</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Minúsculas</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Subscrito</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Sobrescrito</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Tachado</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation>Contorno do Texto</translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>Update paragraph styles</source>
        <translation>Atualizar estilo de parágrafos</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Use o nome do documento como prefixo para 
estilos de parágrafo</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Não perguntar novamente</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation>O importador deverá adicionar o nome do documento 
 em frente ao nome do estilo de parágrafo no Scribus</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation>Se um estilo de parágrafo atual existir com o mesmo 
nome do documento do OpenOffice.org, o estilo 
no Scribus deverá ser alterado para aproximar-se 
com o que está sendo importado ou sair sem alterações</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>OpenOffice.org Write - Opções de Importação</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation>Deverá o importador sempre usar o valor atual quando 
importar documentos do OpenOffice.org e nunca mais 
pedir sua confirmação novamente</translation>
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
        <translation>Gerenciar Tabuladores</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>Tabruler</name>
    <message>
        <source>Left</source>
        <translation>Esquerda</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Direita</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Parar totalmente</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Vírgula</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Centro</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Apagar Tudo</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Distância da margem entre a primeira linha do parágrafo</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Distância da margem de esquerda para o resto do parágrafo</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Apaguar todas as tabulações 
</translation>
    </message>
    <message>
        <source> pt</source>
        <translation> pontos</translation>
    </message>
    <message>
        <source> mm</source>
        <translation>mm</translation>
    </message>
    <message>
        <source> in</source>
        <translation> em</translation>
    </message>
    <message>
        <source> p</source>
        <translation>picas</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>&amp;Posição:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation>Primeira &amp;Linha:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation>Ma&amp;rgem esquerda:</translation>
    </message>
</context>
<context>
    <name>Tree</name>
    <message>
        <source>X:</source>
        <translation>X:</translation>
    </message>
    <message>
        <source>Y:</source>
        <translation>Y:</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Linha</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Página</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation>Fonte:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation>Imagem</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Polyline</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Polígono</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Caminho de Texto</translation>
    </message>
    <message>
        <source>Information</source>
        <translation>Informação</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Aviso</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Nome %1 não é único. 
Por favor, selecione outro.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Grupo</translation>
    </message>
</context>
<context>
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Inserir valor</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Entre com um valor e então pressione OK.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Entre com um valor e pressione OK</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O </translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Envie um valor para o script</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="unfinished">&amp;Cancelar</translation>
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
        <translation>Ferramentas</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation>Inserir Linhas</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation>Inserir Imagem</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation>Selecionar Ítens</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation>Editar Conteúdo do Quadro</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation>Inserir Curvas Bezier</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation>Rotacionar Ítem</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Propriedades...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation>Inserir Polígonos</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation>Inserir Quadro de Texto</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation>Inserir Tabela</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation>Inserir linha do Freehand </translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation>Ampliar ou Reduzir</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation>Editar o Texto com o Editor de Texto</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation>Ligar Quadros de Texto</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation>Separar Quadros de Texto</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation>Desenhar Várias Formas</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation>Exibir medidas</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Link</source>
        <translation>Ligação</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Caixa de Lista</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Caixa de Combinação</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Botão</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Campo de Texto</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Caixa de Checagem </translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>Ferramentas de PDF</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Inserir campos de PDF</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Inserir anotações de PDF</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation>Selecionar letra:</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation>&amp;Inserir</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Fechar</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation>Insere caracteres no texto a partir do cursor</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation>Apagar a(s) seleção(ões) atual(is).</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation>Fecha essa janela e retorna para a edição de texto.</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Escolha um importador</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Automática</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation>Apenas obter texto</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Importar texto sem nenhuma formatação</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Importador:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Codificação:</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Escolha um importador </translation>
    </message>
    <message>
        <source></source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Lembrar associação</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation>Lembrar da extensão do arquivo - importar 
associação e não perguntar novamente para selecionar 
um importador  para arquivos desse tipo.</translation>
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
        <translation>Novo a partir de um modelo</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>&amp;Remover</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Abrir</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Todo</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Cores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descrição</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Uso</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Criado com</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Autor</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK </translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Adquirindo Modelos </translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Instalando Modelo</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Extraia o arquivo para o diretório de modelos 
~/.scribus/template para o usuário atual ou 
PREFÍXO/share/scribus/templates para todos os 
 usuário do sistema.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Preparando modelo</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Removendo modelo</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Traduzindo modelo.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Modelos de documentos podem ser encontrados 
em http://www.sciburs.net/ na seção de Downloads.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Tenha certeza que as imagens e fontes utilizadas 
sejam de uso livre. Se as fonte não poderem ser 
 compartilhadas, não as adicione quando salvar 
como modelo.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>O criador de modelo deverá também ter certeza que 
 o Instalador de Modelos poderá aplicar as mudanças 
nos modelos dele também. Isso significa que um usuário 
deverá ser hábil a fazer o download, extrair para o 
diretório de modelos e começar a usá-lo.
</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Removendo uma modelo do diálogo &quot;Novo a partir de um modelo&quot; apenas removerá a entrada do modelo.xml, isso não apagará os arquivos. Um menu popup com a opção remover só é mostrado se você tiver acesso a escrita ao arquivo modelo.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Copie um modelo.xml existente para um arquivo chamado modelo.lang_COUNTRY.xml (use o mesmo código de idioma que está presente no arquivo qm de sua linguagem), por exemplo modelo.fi.xml para  finlandês. A cópia deve estar localizado no mesmo diretório do modelo original assim, o Scribus poderá carrega-lo.
</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Data</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Salvar como modelo</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Nome</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Categoria</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Tamanho da Página</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Cores</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Descrição</translation>
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
        <translation>Email </translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Mais detalhes</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>OK</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Menos detalhes</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Legal </translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Carta</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Tablóide 
</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>paisagem</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>retrato</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>personalizado</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Criar filtro</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>&amp;Limpar</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Apagar</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Selecione um filtro salvo anteriormente</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Dê um nome a esse filtro para salvar</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Dê um nome ao arquivo</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation>&amp;OK 
</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Desabilitar ou habilitar esse filtro de linha</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Remover esse filtro de linha</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Adicionar novo filtro de linha</translation>
    </message>
    <message>
        <source>to</source>
        <translation>para</translation>
    </message>
    <message>
        <source>and</source>
        <translation>e</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>remover caso</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>não remover caso</translation>
    </message>
    <message>
        <source>words</source>
        <translation>palavras</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Remover</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Substituir</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Aplicar</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>O valor  à esquerda é uma expressão regular</translation>
    </message>
    <message>
        <source>with</source>
        <translation>com</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>estilo de parágrafo</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>todas as instâncias de</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>todos os parágrafos</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>parágrafos começando com</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>parágrafos com menos do que</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>fparágrafos com mais do que </translation>
    </message>
</context>
</TS>
