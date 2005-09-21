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

Повертає розмір шрифта в тчк для текстової рамки з назвою &quot;name&quot;. Якщо в цій
текстовій рамці вибрано частину тексту, то функція повертає значення для
першого знака вибірки. Якщо &quot;name&quot; не вказане, буде використано активний об&apos;єкт.</translation>
    </message>
    <message>
        <source>getColorNames() -&gt; list

Returns a list containing the names of all defined colors in the document.
If no document is open, returns a list of the default document colors.
</source>
        <translation>getColorNames() -&gt; list

Повертає список імен всіх кольорів визначених в документі.
Якщо відкритий документ відсутній, повертає список основних
кольорів.</translation>
    </message>
    <message>
        <source>newDocDialog() -&gt; bool

Displays the &quot;New Document&quot; dialog box. Creates a new document if the user
accepts the settings. Does not create a document if the user presses cancel.
Returns true if a new document was created.
</source>
        <translation>newDocDialog() -&gt; bool

Показує діалогове вікно &quot;Новий Документ&quot;. Створює новий документ, якщо
користувач підтвердить установки. Не створює документ, якщо користувач
вибирає кнопку &quot;Вихід&quot;. Повернене значення дорівнює булевому &quot;так&quot;, якщо 
документ було створено.</translation>
    </message>
    <message>
        <source>getFillColor([&quot;name&quot;]) -&gt; string

Returns the name of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillColor([&quot;name&quot;]) -&gt; string

Повертає ім&apos;я кольору заповнення об&apos;єкту під назвою &apos;name&apos;.
Якщо &quot;name&quot; не дано, то використовується активний об&apos;єкт.</translation>
    </message>
    <message>
        <source>moveObject(dx, dy [, &quot;name&quot;])

Moves the object &quot;name&quot; by dx and dy relative to its current position. The
distances are expressed in the current measurement unit of the document (see
UNIT constants). If &quot;name&quot; is not given the currently selected item is used.
If the object &quot;name&quot; belongs to a group, the whole group is moved.
</source>
        <translation>moveObject(dx, dy [, &quot;name&quot;])

Зсовує об&apos;єкт &quot;name&quot; на dx і dy відносно до його теперішньої позиції. Відстані
визначаються активними одиницями виміру документа (див. константи Юнікс).
Якщо &quot;name&quot; не дано, то використовується активний об&apos;єкт. Якщо об&apos;єкт &quot;name&quot; 
належить до групи, то рухається вся група.
</translation>
    </message>
    <message>
        <source>setRedraw(bool)

Disables page redraw when bool = False, otherwise redrawing is enabled.
This change will persist even after the script exits, so make sure to call
setRedraw(True) in a finally: clause at the top level of your script.
</source>
        <translation>setRedraw(bool)

Відключає обновлення сторінки, якщо bool = False, в протилежному
випадку обновлення ввімкнене. Ця установка залишається в силі 
навіть після закінчення роботи сценарія. Тому в кінці сценарія
необхідно помістити виклик setRedraw(True) в умові finally: в верхньому
рівні сценарія.</translation>
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

Створює новий прямокутник на активній сторінці і повертаю його ім&apos;я. Координати
задаються в активних одиницях виміру документа (див. константи Юнікс). &quot;name&quot; 
має унікально ідентифікувати об&apos;єкт, тому що воно буде необхідним для звернення
до цього об&apos;єкта в майбутньому. Якщо &quot;name&quot; не вказане, то Scribus вибере його
сам.

Може повернути помилку NameExistsError якщо ви вкажете ім&apos;я, яке вже було використане.
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

Створює нову сторінку. Якщо &quot;where&quot; дорівнює -1 то нова сторінка добавляється
в кінець документа. В іншому випадку нова сторінка вставляється перед &quot;where&quot;.
Номери сторінок рахуються від 1 вверх, незалежно від того який номер показаний
на першій сторінці Вашого документа. Необов&apos;язковий параметр &quot;template&quot; визначає
ім&apos;я шаблона для нової сторінки.

Може повернути помилку IndexError, якщо номер сторінки виходить за дозволений
діапазон
</translation>
    </message>
    <message>
        <source>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Sets the gradient fill of the object &quot;name&quot; to type. Color descriptions are
the same as for setFillColor() and setFillShade(). See the constants for
available types (FILL_&lt;type&gt;).
</source>
        <translation>setGradientFill(type, &quot;color1&quot;, shade1, &quot;color2&quot;, shade2, [&quot;name&quot;])

Установлює заповнення градієнтом об&apos;єкта &quot;name&quot; типом type. Описання
кольорів застосовуються так же як і в setFillColor() та setFillShade(). Дивіться
константи для доступних типів (FILL_&lt;type&gt;).
</translation>
    </message>
    <message>
        <source>messagebarText(&quot;string&quot;)

Writes the &quot;string&quot; into the Scribus message bar (status line). The text
must be UTF8 encoded or &apos;unicode&apos; string(recommended).
</source>
        <translation>messagebarText(&quot;string&quot;)

Поміщає строкову величину &quot;string&quot; в панель повідомлень Scribus (полоса статусу). 
Текст має бути в UTF8 кодуванні або строковою величиною &apos;unicode&apos; (рекомендовано).
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

Створює новий документ та повертає true в разі успіху. Параметри мають
слідуюче значення:

    розмір - size = Тупль (ширина,висота), який описує розмір документу. Ви 
    можете використовувати попередньо визначені константи названі 
    PAPER_&lt;paper_type&gt;, наприклад PAPER_A4 і.т.д.

    поля - margins = Тупль (ліве, праве, верхнє, нижнє) - описує поля документа.

    орієнтація - orientation = орієнтація сторінки - константи PORTRAIT - портретна,
    LANDSCAPE - ландшафтна.

    firstPageNumber = номер першої сторінки даного документа. Звичайно 
    починається з 1, але більші значення можуть використовуваться, наприклад 
    якщо Ви створюєте документ, який складається з кількох частин.

    одиниця виміру - unit: це значення установлює одиниці виміру, які будуть
    використовуватися в документі. Використовуйте попередньо визначені
    константи: UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.

    книжкові сторінки - facingPages = FACINGPAGES, NOFACINGPAGES

    перша сторона зліва - firstSideLeft = FIRSTPAGELEFT, FIRSTPAGERIGHT

Значення для ширини, висоти, та полів виражаються в одиницях виміру, вибраних
для всього документа. PAPER_* константи виражаються в точках. Якщо Ваш документ
не вимірюється в точках, майте це на увазі.

Приклад: newDoc(PAPER_A4, (10, 10, 20, 20), LANDSCAPE, 1, UNIT_POINTS,
                FACINGPAGES, FIRSTPAGERIGHT)
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

Створити новий макрос з іменем &quot;name&quot; з існуючим виконуваним 
об&apos;єктом &quot;callable&quot;. Виконуваний об&apos;єкт не повинен вимагати ніяких 
аргументів при його виклику (він може опрацьовувати необов&apos;язкові
аргументи, але вони йому не будуть дані).
Якщо задана, строкова величина &quot;accel&quot; буде використана для створення
гарячої послідовності для макроса.
Якщо виконуваний елемент являється класом, його буде відторгнено.
Дозволяються функції та зв&apos;язані методи а також ініціалізовані класи 
які мають метод __call__, що не приймає аргументів. Виконуваний об&apos;єкт
можна реєстровати кілька разів. Також дозволяється реєструвати 
декілька зв&apos;язаних методів одного ініціалізованого класу.</translation>
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
        <translation type="unfinished"></translation>
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

Повертає ім&apos;я шрифта в текстовій рамці &quot;name&quot;. Якщо частина текста
в цій рамці виділена, то повертає значення для першого знака вибірки.
Якщо &quot;name&quot; не вказане - повертає значення для активного об&apos;єкту.</translation>
    </message>
    <message>
        <source>getTextLength([&quot;name&quot;]) -&gt; integer

Returns the length of the text in the text frame &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getTextLength([&quot;name&quot;]) -&gt; integer

Повертає довжину тексту в текстовій рамці з ім&apos;ям &quot;name&quot;.
Якщо &quot;name&quot; не вказане, то використовується активний
об&apos;єкт.</translation>
    </message>
    <message>
        <source>getText([&quot;name&quot;]) -&gt; string

Returns the text of the text frame &quot;name&quot;. If this text frame has some text
selected, the selected text is returned. All text in the frame, not just
currently visible text, is returned. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>getText([&quot;name&quot;]) -&gt; string

Повертає текст з текстової рамки &quot;name&quot;. Якщо в цій рамці частина тексту
виділена, то повертає виділений текст. Повертає весь текст в рамці, а не 
тільки видиму частину. Якщо &quot;name&quot; не вказане, то використовується
активний об&apos;єкт.
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

Повертає текст із текстової рамки &quot;name&quot; і всіх текстових рамок зв&apos;язаних з нею.
Якщо в цій рамці частина тексту виділена, то повертає виділений текст. Якщо 
&quot;name&quot; не вказано, то використовується активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getLineSpacing([&quot;name&quot;]) -&gt; float

Returns the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; expressed in
points. If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineSpacing([&quot;name&quot;]) -&gt; float

Повертає міжрядковий інтервал (&quot;leading&quot;) текстової рамки з ім&apos;ям &quot;name&quot;,
виражений в точках. Якщо &quot;name&quot; не вказано, то використовується активний
об&apos;єкт.</translation>
    </message>
    <message>
        <source>getColumnGap([&quot;name&quot;]) -&gt; float

Returns the column gap size of the text frame &quot;name&quot; expressed in points. If
&quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getColumnGap([&quot;name&quot;]) -&gt; float

Повертає розмір інтервалу між колонками тексту в текстовій рамці &quot;name&quot;, 
виражений в точках. Якщо &quot;name&quot; не задано, використовується активний
об&apos;єкт.</translation>
    </message>
    <message>
        <source>getColumns([&quot;name&quot;]) -&gt; integer

Gets the number of columns of the text frame &quot;name&quot;. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getColumns([&quot;name&quot;]) -&gt; integer

Повертає число колонок тексту в текстовій рамці &quot;name&quot;. Якщо &quot;name&quot; 
не вказано, використовується активний об&apos;єкт.</translation>
    </message>
    <message>
        <source>setText(&quot;text&quot;, [&quot;name&quot;])

Sets the text of the text frame &quot;name&quot; to the text of the string &quot;text&quot;.
Text must be UTF8 encoded - use e.g. unicode(text, &apos;iso-8859-2&apos;). See the FAQ
for more details. If &quot;name&quot; is not given the currently selected item is
used.
</source>
        <translation>setText(&quot;text&quot;, [&quot;name&quot;])

Змінює текст в текстовій рамці &quot;name&quot; на зміст текстової змінної &quot;text&quot;.
Текст має бути в кодуванні UTF8 - використовуйте, наприклад, 
unicode(text, &apos;iso-8859-2&apos;). Додаткова інформація по цьому приводу
розміщена в часто задаваних питаннях. Якщо &quot;name&quot; не вказано, то
використовується активний об&apos;єкт.</translation>
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

Вставляє текст&quot; text&quot; в позицію &quot;pos&quot; в текстову рамку. Текст має бути в
кодуванні UTF (дивіться setText() для подальшої інформації). Перший знак
має індекс 0. Якщо &quot;name&quot; не вказано, використовується активний об&apos;єкт.

Може повернути помилку IndexError якщо індекс вставки виходить за 
дозволений діапазон.
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

Установлює шрифт текстової рамки &quot;name&quot; до &quot;font&quot;. Якщо вибрана лише 
частина тексту в рамці то зміна шрифта відноситься лише до неї. Якщо  
&quot;name&quot; не вказано, то використовується активний об&apos;єкт.

Може повернути ValueError, якщо шрифт не може бути знайдено.
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

Установлює розмір шрифта в текстовій рамці &quot;name&quot; до розміру &quot;size&quot;. 
Значення &quot;size&quot; визначається в точках. Якщо в рамці вибрана частина
тексту, то зміна розміру шрифта відноситься лише до неї. Діапазон &quot;size&quot; 
від 1 до 512. Якщо &quot;name&quot; не вказано, то використовується активний
об&apos;єкт.

Може повернути помилку ValueError, якщо розмір шрифта виходить за
дозволений діапазон.</translation>
    </message>
    <message>
        <source>setLineSpacing(size, [&quot;name&quot;])

Sets the line spacing (&quot;leading&quot;) of the text frame &quot;name&quot; to &quot;size&quot;.
&quot;size&quot; is a value in points. If &quot;name&quot; is not given the currently selected
item is used.

May throw ValueError if the line spacing is out of bounds.
</source>
        <translation>setLineSpacing(size, [&quot;name&quot;])

Установлює міжрядковий інтервал (початковий) в текстовій рамці &quot;name&quot;
до розміру &quot;size&quot;. Значення &quot;size&quot; виражається в точках. Якщо &quot;name&quot; не
вказано, використовується активний об&apos;єкт.

Може повернути помилку ValueError, якщо значення інтервалу виходить
за доступний діапазон.
</translation>
    </message>
    <message>
        <source>setColumnGap(size, [&quot;name&quot;])

Sets the column gap of the text frame &quot;name&quot; to the value &quot;size&quot;. If
&quot;name&quot; is not given the currently selected item is used.

May throw ValueError if the column gap is out of bounds (must be positive).
</source>
        <translation>setColumnGap(size, [&quot;name&quot;])

Установлює проміжок між колонками в текстовій рамці &quot;name&quot; до значення
&quot;size&quot;. Якщо &quot;name&quot; не задано, використовується активний об&apos;єкт.

Може повернути помилку ValueError, якщо проміжок між колонками виходить
за доступний діапазон (повинен бути позитивним).
</translation>
    </message>
    <message>
        <source>setColumns(nr, [&quot;name&quot;])

Sets the number of columns of the text frame &quot;name&quot; to the integer &quot;nr&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May throw ValueError if number of columns is not at least one.
</source>
        <translation>setColumns(nr, [&quot;name&quot;])

Установлює число колонок в текстовій рамці &quot;name&quot; до цілого числа &quot;nr&quot;.
Якщо &quot;name&quot; не задано, використовується активний об&apos;єкт.

Може повернути помилку ValueError, якщо не вказана хоч одна колонка.
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

Установлює вирівнювання тексту в текстовій рамці &quot;name&quot; до вказаного.
Якщо &quot;name&quot; не вказано, використовується активний об&apos;єкт. &quot;align&quot; повинне
дорівнювати одній з констант ALIGN_ constants визначених в цьому модулі -
дивіться в dir(scribus).

Може повернути помилку ValueError, якщо константа вирівнювання задана
невірно.
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
        <translation type="unfinished">selectText(start, count, [&quot;name&quot;])

Вибирає &quot;count&quot; число знаків тексту в текстовій рамці &quot;name&quot;, починаючи з
знака &quot;start&quot;. Рахунок знаків починається з 0. Якщо &quot;count&quot; дорівнює нулю,
текстова вибірка буде знята. Якщо &quot;name&quot; не задано, буде використано
активний об&apos;єкт.

Може повернути помилку IndexError, якщо вибірка виходить за межі 
наявного тексту.
</translation>
    </message>
    <message>
        <source>deleteText([&quot;name&quot;])

Deletes any text in the text frame &quot;name&quot;. If there is some text selected,
only the selected text will be deleted. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>deleteText([&quot;name&quot;])

Видаляє весь текст в текстовій рамці &quot;name&quot;. Якщо частина тексту вибрана
то видаляється лише вона. Якщо &quot;name&quot; не задано, використовується
активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Sets the text color of the text frame &quot;name&quot; to the color &quot;color&quot;. If there
is some text selected only the selected text is changed. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>setTextColor(&quot;color&quot;, [&quot;name&quot;])

Установлює колір тексту в текстовій рамці &quot;name&quot; до кольору &quot;color&quot;. Якщо
частина тексту вибрана - змінюється лише вона. Якщо &quot;name&quot; не задано,
використовується активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Set &quot;color&quot; of the text stroke. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation>setTextStroke(&quot;color&quot;, [&quot;name&quot;])

Установлює колір &quot;color&quot; текстового контура. Якщо &quot;name&quot; не задано,
використовується активний об&apos;єкт.
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

Установлює тінь кольору тексту об&apos;єкта &quot;name&quot; до &quot;shade&quot;. Якщо
частина тексту вибрана - змінюється лише вона. &quot;shade&quot; має бути
цілим числом від 0 (найсвітліша) до 100 (повна насиченість кольору).
Якщо &quot;name&quot; не задано, використовується активний об&apos;єкт.
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

Зв&apos;язує дві текстові рамки. Рамка з ім&apos;ям &quot;fromname&quot; приєднується до
рамки з ім&apos;ям &quot;toname&quot;. Кінцева рамка має бути пустою текстовою
рамкою і не повинна бути з&apos;єднана ні з якою іншою рамкою.

Може повернути помилку ScribusException, якщо відбувається порушення
правил зв&apos;язування рамок.
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

Видаляє вказаний (названий) об&apos;єкт з ланцюжка зв&apos;язаних текстових
рамок. Якщо рамка знаходиться всередині ланцюжка, то рамки, які
знаходилися перед і після неї будуть з&apos;єднані, наприклад &apos;a-&gt;b-&gt;c&apos; 
зміниться на &apos;a-&gt;c&apos;, при виконанні unlinkTextFrames(b)&apos;

Може повернути помилку ScribusException при порушенні правил
з&apos;єднання рамок.
</translation>
    </message>
    <message>
        <source>traceText([&quot;name&quot;])

Convert the text frame &quot;name&quot; to outlines. If &quot;name&quot; is not given the
currently selected item is used.</source>
        <translation>traceText([&quot;name&quot;])

Конвертує текстову рамку &quot;name&quot; в контурний текст. Якщо &quot;name&quot; 
не вказано, використовується активний об&apos;єкт.</translation>
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

Повертає тупль (C, M, Y, K), який містить чотири складових
кольору &quot;name&quot; з активного документа. Якщо не відкрито
жодного документа, повертає значення для названого
кольору з палітри стандартних кольорів.

Може повернути помилку NotFoundError, якщо названий колір не знайдено.
Може повернути помилку ValueError, якщо було вказано недійсне ім&apos;я кольору.
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

Змінює колір &quot;name&quot; до вказаних значень CMYK. Значення кольору
визначається його чотирма компонентами c = Cyan (салатовий),
m = Magenta (малиновий), y = Yellow (жовтий) та k = Black (чорний).
Значення складових кольору повинні знаходитися в діапазоні від
0 до 255.

Може повернути помилку NotFoundError, якщо названий колір не знайдено.
Може повернути помилку ValueError, якщо було вказано недійсне ім&apos;я кольору.
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

Видаляє колір &quot;name&quot;. Кожний приклад цього кольору заміняється на колір
&quot;replace&quot;. Якщо &quot;replace&quot; не вказано, то використовується колір &quot;None&quot; - прозорість.

deleteColor змінює стандартні кольори при відсутності відчинених документів. В
цьому випадку &quot;replace&quot;, якщо вказаний, ефекту не має.

Може повернути помилку NotFoundError, якщо названий колір не знайдено.
Може повернути помилку ValueError, якщо було вказано недійсне ім&apos;я кольору.</translation>
    </message>
    <message>
        <source>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Every occurence of the color &quot;name&quot; is replaced by the color &quot;replace&quot;.

May raise NotFoundError if a named color wasn&apos;t found.
May raise ValueError if an invalid color name is specified.
</source>
        <translation>replaceColor(&quot;name&quot;, &quot;replace&quot;)

Кожний приклад кольору &quot;name&quot; заміняється на колір &quot;replace&quot;.

Може повернути помилку NotFoundError, якщо названий колір не знайдено.
Може повернути помилку ValueError, якщо було вказано недійсне ім&apos;я кольору.</translation>
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
        <translation type="obsolete">fileDialog(&quot;caption&quot;, [&quot;filter&quot;, &quot;defaultname&quot; ,haspreview, issave]) -&gt; строкова
величина з іменем файла

Показує діалогове вікно &quot;Відчинити файл&quot; з аннотацією &quot;caption&quot;. Імена 
файлів фільтруються за домомогою строкової змінної &quot;filter&quot;. Може бути вказане стандартне ім&apos;я або шлях до файла (&quot;defaultname&quot;) - залиште цю змінну  пустою,
якщо Ви не бажаєте її використовувати. Значення &quot;True&quot; для &quot;haspreview&quot;
вмикає застосування невеликого віджета попереднього перегляду в діалоговому
вікні &quot;FileSelect&quot;. Якщо параметр &quot;issave&quot; дорівнює &quot;True&quot;, діалогове вікно діє, як
&quot;Записати Як&quot;, в іншому разі воно відповідає &quot;Відчинити файл&quot;. Значення по
умовчанню для обох змінних дорівнює &quot;False&quot;.

Якщо вказане значення фільтра, воно повинне мати вид &apos;коментар (*.type *.type2 ...)&apos;.
Наприклад &apos;Images (*.png *.xpm *.jpg)&apos;.

Зверніться до документації по Qt для QFileDialog за детальним описом фільтрів.

Приклад: fileDialog(&apos;Open input&apos;, &apos;CSV files (*.csv)&apos;)
Приклад: fileDialog(&apos;Save report&apos;, defaultname=&apos;report.txt&apos;, issave=True)</translation>
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
    button2=BUTTON_NONE, button3=BUTTON_NONE) -&gt; ціле число

Показує діалогове вікно з заголовком &quot;caption&quot;, повідомленням &quot;message&quot;, 
та іконкою &quot;icon&quot; та до 3-х кнопок. По умовчанню іконка не використовується, 
присутня лише проста кнопка &quot;Гаразд&quot;. Обов&apos;язковими являються лише 
аргументи &quot;caption&quot; та &quot;message&quot;, хоча установка іконки та відповідних
кнопок настійливо рекомендується. Текст повідомлення може містити
просту розмітку по типу HTML.

Повертає номер кнопки, натиснутої користувачем. Номера кнопок
починаються з 1.

Для іконок та кнопок існують наперед визначені константи з іменами 
описаними в документації по Qt. Вони включають BUTTON_* та
ICON_* константи, визначені в модулі. Додатково є ще дві константи,
для яких може бути виконана бінарна логічна операція OR з константами
кнопок
    BUTTONOPT_DEFAULT  Натискування клавіші &quot;Ввід&quot; (Enter) натискує цю кнопку.
    BUTTONOPT_ESCAPE    Натискування клавіші &quot;Відміна&quot; (Escape) натискує цю кнопку.

Приклади використання:
result = messageBox(&apos;Script failed&apos;,
                    &apos;Цей скрипт працює лише при наявності вибраної текстової рамки.&apos;,
                    ICON_ERROR)
result = messageBox(&apos;Мавпи!&apos;, &apos;Сталося щось негарне! &lt;i&gt;Це була мавпа?&lt;/i&gt;&apos;,
                    ICON_WARNING, BUTTON_YES|BUTTONOPT_DEFAULT,
                    BUTTON_NO, BUTTON_IGNORE|BUTTONOPT_ESCAPE)

Визначені константи кнопок та іконок:
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
        <translation>valueDialog(caption, message [,defaultvalue]) -&gt; строкова величина

Показує стандартний &apos;Введіть строкову величину&apos; діалог та повертає
отримане значення, як строкову величину
Параметри: заголовок вікна, текст у вікні та необов&apos;язкове &apos;значення 
по умовчанню&apos; (default).

Приклад: valueDialog(&apos;заголовок&apos;, &apos;текст у вікні&apos;, &apos;необов&quot;язкове значення&apos;)
</translation>
    </message>
    <message>
        <source>closeDoc()

Closes the current document without prompting to save.

May throw NoDocOpenError if there is no document to close
</source>
        <translation>closeDoc()

Зачиняє активний документ без запиту про запис.

Може повернути помилку NoDocOpenError при відсутності
документа для зачиняння
</translation>
    </message>
    <message>
        <source>haveDoc() -&gt; bool

Returns true if there is a document open.
</source>
        <translation>haveDoc() -&gt; булеве значення

Повертає правдиве булеве значення при 
наявності відчиненого документа.</translation>
    </message>
    <message>
        <source>openDoc(&quot;name&quot;)

Opens the document &quot;name&quot;.

May raise ScribusError if the document could not be opened.
</source>
        <translation>openDoc(&quot;name&quot;)

Відчиняє документ &quot;name&quot;.

Може повернути помилку ScribusError, якщо документ не можна
було відчинити.
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

Записує активний документ під його теперішнім іменем, повертає
правдиве булеве значення при успішному завершенні операції.
Якщо документ ще не був записаний, може викликати інтерактивний
діалог збереження файла.

В даний час неможливо визначити чи запис був успішним.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;name&quot;)

Saves the current document under the new name &quot;name&quot; (which may be a full or
relative path).

May raise ScribusError if the save fails.
</source>
        <translation>saveDocAs(&quot;name&quot;)

Записує активний документ під новим ім&apos;ям &quot;name&quot;, яке може бути
повним або відносним шляхом.

Може повернути помилку ScribusError, якщо запис не буде успішним.
</translation>
    </message>
    <message>
        <source>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; bool

Sets the document information. &quot;Author&quot;, &quot;Info&quot;, &quot;Description&quot; are
strings.
</source>
        <translation>saveDocAs(&quot;author&quot;, &quot;info&quot;, &quot;description&quot;) -&gt; булеве значення

Установлює опис документа. &quot;Author (&quot;Автор&quot;), &quot;Info&quot; (&quot;Інформація&quot;),
&quot;Description&quot; (&quot;Опис&quot;) є строковими величинами.
</translation>
    </message>
    <message>
        <source>setMargins(lr, rr, tr, br)

Sets the margins of the document, Left(lr), Right(rr), Top(tr) and Bottom(br)
margins are given in the measurement units of the document - see UNIT_&lt;type&gt;
constants.
</source>
        <translation>setMargins(lr, rr, tr, br)

Установлює поля документа, Ліве (lr), Праве (rr), Верхнє (tr) та Нижнє (br)
поля задаються в одиницях виміру документа - див. UNIT_&lt;type&gt;
константи.
</translation>
    </message>
    <message>
        <source>setUnit(type)

Changes the measurement unit of the document. Possible values for &quot;unit&quot; are
defined as constants UNIT_&lt;type&gt;.

May raise ValueError if an invalid unit is passed.
</source>
        <translation>setUnit(type)

Установлює одиницю виміру документа. Можливі значення для &quot;unit&quot; 
визначаються як константи UNIT_&lt;type&gt;.

Може повернути помилку ValueError, якщо було задано невірну одиницю
виміру.
</translation>
    </message>
    <message>
        <source>getUnit() -&gt; integer (Scribus unit constant)

Returns the measurement units of the document. The returned value will be one
of the UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</source>
        <translation>getUnit() -&gt; integer (Scribus unit constant)

Повертає одиниці виміру документа. Повернене значення буде однією з
констант UNIT_* constants:
UNIT_INCHES, UNIT_MILLIMETERS, UNIT_PICAS, UNIT_POINTS.
</translation>
    </message>
    <message>
        <source>loadStylesFromFile(&quot;filename&quot;)

Loads paragraph styles from the Scribus document at &quot;filename&quot; into the
current document.
</source>
        <translation>loadStylesFromFile(&quot;filename&quot;)

Завантажує стилі абзаців з документу Scribus &quot;filename&quot; в активний документ.
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

Установлює тип документа. Для задання книжкового розташування сторінок
установіть перший параметр до FACINGPAGES, а для відміни замість facingPages 
використайте NOFACINGPAGES. Щоб перша сторінка була зліва установіть
другий параметр до FIRSTPAGELEFT, а справа - FIRSTPAGERIGHT.
</translation>
    </message>
    <message>
        <source>getLineColor([&quot;name&quot;]) -&gt; string

Returns the name of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineColor([&quot;name&quot;]) -&gt; строкове значення

Повертає назву кольорі ліній об&apos;єкту &quot;name&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getLineWidth([&quot;name&quot;]) -&gt; integer

Returns the line width of the object &quot;name&quot;. If &quot;name&quot;
is not given the currently selected Item is used.
</source>
        <translation>getLineWidth([&quot;name&quot;]) -&gt; ціле число

Повертає товщину лінії об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot;
не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getLineShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the line color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getLineShade([&quot;name&quot;]) -&gt; ціле число

Повертає значення затінення об&apos;єкта &quot;name&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getLineJoin([&quot;name&quot;]) -&gt; integer (see contants)

Returns the line join style of the object &quot;name&quot;. If &quot;name&quot; is not given
the currently selected item is used.  The join types are:
JOIN_BEVEL, JOIN_MITTER, JOIN_ROUND
</source>
        <translation>getLineJoin([&quot;name&quot;]) -&gt; ціле число (див. умови)

Повертає стиль з&apos;єднань ліній об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; не
задано, буде використано активний об&apos;єкт. Типи з&apos;єднать:
JOIN_BEVEL (фасочне), JOIN_MITTER (фацеточне), JOIN_ROUND (закруглене)
</translation>
    </message>
    <message>
        <source>getLineEnd([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line cap style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. The cap types are:
CAP_FLAT, CAP_ROUND, CAP_SQUARE
</source>
        <translation>getLineEnd([&quot;name&quot;]) -&gt; ціле число (див. умови)

Повертає стиль кінців ліній об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; не
задано, буде використано активний об&apos;єкт. Типи кінців:
CAP_FLAT (плаский), CAP_ROUND (заокруглений), CAP_SQUARE (квадратний)
</translation>
    </message>
    <message>
        <source>getLineStyle([&quot;name&quot;]) -&gt; integer (see constants)

Returns the line style of the object &quot;name&quot;. If &quot;name&quot; is not given the
currently selected item is used. Line style constants are:
LINE_DASH, LINE_DASHDOT, LINE_DASHDOTDOT, LINE_DOT, LINE_SOLID
</source>
        <translation>getLineStyle([&quot;name&quot;]) -&gt; ціле число (див. обмеження)

Повертає стиль ліній об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; не задано, буде 
використано активний об&apos;єкт. Обмеження типів ліній:
LINE_DASH (пунктирна), LINE_DASHDOT (пунктир-крапка), 
LINE_DASHDOTDOT (пунктир-крапка-крапка), LINE_DOT (крапка),
LINE_SOLID (суцільна)
</translation>
    </message>
    <message>
        <source>getFillShade([&quot;name&quot;]) -&gt; integer

Returns the shading value of the fill color of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getFillShade([&quot;name&quot;]) -&gt; integer

Повертає значення затінення кольору заповнення об&apos;єкта &quot;name&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple containing the scaling values of the image frame
&quot;name&quot;.  If &quot;name&quot; is not given the currently selected item is used.
</source>
        <translation>getImageScale([&quot;name&quot;]) -&gt; (x,y)

Повертає тупль (x, y), який містить значення масштабу рамки для зображень
&quot;name&quot;.  Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getImageName([&quot;name&quot;]) -&gt; string

Returns the filename for the image in the image frame. If &quot;name&quot; is not
given the currently selected item is used.
</source>
        <translation>getImageName([&quot;name&quot;]) -&gt; строкове значення

Повертає ім&apos;я файла для зображення в рамці для зображень. Якщо &quot;name&quot; 
не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getSize([&quot;name&quot;]) -&gt; (width,height)

Returns a (width, height) tuple with the size of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used. The size is
expressed in the current measurement unit of the document - see UNIT_&lt;type&gt;
for reference.
</source>
        <translation>getSize([&quot;name&quot;]) -&gt; (ширина,висота)

Повертає тупль (ширина,висота) з розміром об&apos;єкта &quot;name&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт. Розмір
виражається в активних одиницях виміру документа - див.  UNIT_&lt;type&gt;
для подальшої інформації.
</translation>
    </message>
    <message>
        <source>getRotation([&quot;name&quot;]) -&gt; integer

Returns the rotation of the object &quot;name&quot;. The value is expressed in degrees,
and clockwise is positive. If &quot;name&quot; is not given the currently selected item
is used.
</source>
        <translation>getRotation([&quot;name&quot;]) -&gt; ціле число

Повертає кут повороту об&apos;єкта &quot;name&quot;. Значення виражається в градусах 
і поворот по часовії стрілці є позитивним. Якщо &quot;name&quot; не задано, буде
використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getAllObjects() -&gt; list

Returns a list containing the names of all objects on the current page.
</source>
        <translation>getAllObjects() -&gt; список

Повертає список, що містить імена всіх об&apos;єктів на активній сторінці.
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

Переміщує об&apos;єкт &quot;name&quot;  в нове місце. Координати виражаються в
активних одиницях виміру документа (див. константи UNIT).  Якщо 
&quot;name&quot; не задано, буде використано активний об&apos;єкт. Якщо об&apos;єкт 
&quot;name&quot; є частиною групи, переміщається вся група.
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

Відносно повертає об&apos;єкт &quot;name&quot; на &quot;rot&quot; градусів. Об&apos;єкт повертається
навколо вортекса, який є активно вибраною точкою обертання. По
умовчанню верхній лівий вортекс при нульовому куті повороту. 
Позитивні значення означають поворот проти часової стрілки при
використанні точки обертання по умовчанню. Якщо &quot;name&quot; не задано, 
буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>sizeObject(width, height [, &quot;name&quot;])

Resizes the object &quot;name&quot; to the given width and height. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>sizeObject(ширина, висота [, &quot;name&quot;])

Змінює розмір об&apos;єкта &quot;name&quot; до вказаної висоти та ширини. Якщо 
&quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getSelectedObject([nr]) -&gt; string

Returns the name of the selected object. &quot;nr&quot; if given indicates the number
of the selected object, e.g. 0 means the first selected object, 1 means the
second selected Object and so on.
</source>
        <translation>getSelectedObject([nr]) -&gt; строкова величина

Повертає ім&apos;я вибраного об&apos;єкта. &quot;nr&quot;, якщо задано, вказує на індекс
вибраного об&apos;єкта, напр. 0 означає перший вибраний об&apos;єкт, 1 означає
другий вибраний об&apos;єкт і так далі.
</translation>
    </message>
    <message>
        <source>selectionCount() -&gt; integer

Returns the number of selected objects.
</source>
        <translation>selectionCount() -&gt; ціле число

Повертає число вибраних об&apos;єктів.
</translation>
    </message>
    <message>
        <source>selectObject(&quot;name&quot;)

Selects the object with the given &quot;name&quot;.
</source>
        <translation>selectObject(&quot;name&quot;)

Повертає об&apos;єкт з даним іменем &quot;name&quot;.
</translation>
    </message>
    <message>
        <source>deselectAll()

Deselects all objects in the whole document.
</source>
        <translation>deselectAll()

Відмінює вибір всіх об&apos;єктів у всьому документі.
</translation>
    </message>
    <message>
        <source>groupObjects(list)

Groups the objects named in &quot;list&quot; together. &quot;list&quot; must contain the names
of the objects to be grouped. If &quot;list&quot; is not given the currently selected
items are used.
</source>
        <translation>groupObjects(list)

Згруповує об&apos;єкти названі в списку &quot;list&quot;. &quot;list&quot; повинен містити імена
об&apos;єктів, які будуть згруповані. Якщо &quot;list&quot; не задано, буде 
використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>unGroupObjects(&quot;name&quot;)

Destructs the group the object &quot;name&quot; belongs to.If &quot;name&quot; is not given the currently selected item is used.</source>
        <translation>unGroupObjects(&quot;name&quot;)

Видаляє групу, яка містить об&apos;єкт з іменем &quot;name&quot;. Якщо &quot;name&quot; не задано, 
буде використано активний об&apos;єкт.</translation>
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

Змінює розмір групи, яка містить об&apos;єкт &quot;name&quot;. Значення понад 1 збільшують 
розмір, а значення менші за 1 його зменшують, напр. значення 0.5 змінює
розмір групи до 50 % початкового, а значення 1.5 змінює розмір до 150 % 
початкового. Значення &quot;factor&quot; повинне бути більшим за 0. Якщо &quot;name&quot; не
задано, буде використано активний об&apos;єкт.

Може повернути помилку ValueError, якщо задано недійсний фактор масштабу.
</translation>
    </message>
    <message>
        <source>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Loads the picture &quot;picture&quot; into the image frame &quot;name&quot;. If &quot;name&quot; is
not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not an image frame
</source>
        <translation>loadImage(&quot;filename&quot; [, &quot;name&quot;])

Завантажує зображення &quot;picture&quot; в рамку для зображень &quot;name&quot;. Якщо 
&quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку WrongFrameTypeError, якщо рамка не є рамкою
для зображень
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

Установлює масштаб зображення в рамці для зображень &quot;name&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт. Число 1
дорівнює 100 %.

Може повернути помилку WrongFrameTypeError, якщо рамка не є 
рамкою для зображень 
</translation>
    </message>
    <message>
        <source>lockObject([&quot;name&quot;]) -&gt; bool

Locks the object &quot;name&quot; if it&apos;s unlocked or unlock it if it&apos;s locked.
If &quot;name&quot; is not given the currently selected item is used. Returns true
if locked.
</source>
        <translation>lockObject([&quot;name&quot;]) -&gt; bool

Замикає об&apos;єкт &quot;name&quot;, якщо він незамкнений, і навпаки. Якщо 
&quot;name&quot; не задано, буде використано активний об&apos;єкт. Повертає
булеве значення &quot;true&quot;, якщо об&apos;єкт замкнений.
</translation>
    </message>
    <message>
        <source>isLocked([&quot;name&quot;]) -&gt; bool

Returns true if is the object &quot;name&quot; locked.  If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>isLocked([&quot;name&quot;]) -&gt; bool

Повертає булеве значення &quot;true&quot;, якщо об&apos;єкт замкнений. Якщо 
&quot;name&quot; не задано, буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getFontNames() -&gt; list

Returns a list with the names of all available fonts.
</source>
        <translation>getFontNames() -&gt; list

Повертає список з назвами доступних шрифтів.
</translation>
    </message>
    <message>
        <source>getXFontNames() -&gt; list of tuples

Returns a larger font info. It&apos;s a list of the tuples with:
[ (Scribus name, Family, Real name, subset (1|0), embed PS (1|0), font file), (...), ... ]
</source>
        <translation>getXFontNames() -&gt; список туплів

Повертає додаткову інформацію про шрифти у вигляді списка туплів:
[ (Назва шрифта в Scribus, гарнітура, справжня назва, вибірка (1|0), 
вбудований PS (1|0), файл з шрифтом), (...), ... ]
</translation>
    </message>
    <message>
        <source>rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Creates an image preview of font &quot;name&quot; with given text &quot;sample&quot; and size.
Image is saved into &quot;filename&quot;. Returns true when success.

May raise NotFoundError if the specified font can&apos;t be found.
May raise ValueError if an empty sample or filename is passed.
</source>
        <translation type="obsolete">rendeFont(&quot;name&quot;, &quot;filename&quot;, &quot;sample&quot;, size) -&gt; bool

Показує зображення шрифта &quot;name&quot; у вигляді заданого тексту
&quot;sample&quot; розміром &quot;size&quot;. Зображення записується в файл &quot;filename&quot;. 
Повертає булеве значення &quot;true&quot; у разі успішного завершення.

Може повернути помилку NotFoundError, якщо вказаний шрифт не знайдено.
Може повернути помилку ValueError, якщо задано пустий текст або назва файла.
</translation>
    </message>
    <message>
        <source>getLayers() -&gt; list

Returns a list with the names of all defined layers.
</source>
        <translation>getLayers() -&gt; list

Повертає список з назвами всіх визначених планів.
</translation>
    </message>
    <message>
        <source>setActiveLayer(&quot;name&quot;)

Sets the active layer to the layer named &quot;name&quot;.

May raise NotFoundError if the layer can&apos;t be found.
May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>setActiveLayer(&quot;name&quot;)

Установлює &quot;name&quot;, як активний план.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
</translation>
    </message>
    <message>
        <source>getActiveLayer() -&gt; string

Returns the name of the current active layer.
</source>
        <translation>getActiveLayer() -&gt; string

Повертає назву активного плану.
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

Переміщує об&apos;єкт &quot;name&quot; на план &quot;layer&quot;. План повинен існувати.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
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

Змінює видимість плану &quot;layer&quot;. Якщо &quot;visible&quot; дорівнює булевому
значенню &quot;false&quot; - план невидимий.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
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

Змінює доступність плану &quot;layer&quot; для друку. Якщо &quot;printable&quot;
дорівнює булевому значенню &quot;false&quot;, план не буде надруковано.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
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
        <translation type="obsolete">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Повертає стан видимості плану &quot;layer&quot; - значення &quot;True&quot; означає, що
план &quot;layer&quot; видимий, а значення &quot;False&quot; означає, що план &quot;layer&quot; 
невидимий.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
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
        <translation type="obsolete">isLayerPrintable(&quot;layer&quot;) -&gt; bool

Повертає стан доступності для друку плану &quot;layer&quot;. При значенні &quot;True&quot;
план &quot;layer&quot; може бути надрукований, а при значенні &quot;False&quot; - ні.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
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

Видаляє план з назвою &quot;layer&quot;. При відсутності плану або якщо це 
останній план документа нічого не відбувається.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плану недопустима.
</translation>
    </message>
    <message>
        <source>createLayer(layer)

Creates a new layer with the name &quot;name&quot;.

May raise ValueError if the layer name isn&apos;t acceptable.
</source>
        <translation>createLayer(layer)

Створює новий план з назвою &quot;name&quot;.

Може повернути помилку ValueError, якщо назва плану недопустима.
</translation>
    </message>
    <message>
        <source>getGuiLanguage() -&gt; string

Returns a string with the -lang value.
</source>
        <translation>getGuiLanguage() -&gt; строкова величина

Повертає строкову величину, яка містить значення -lang.
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
        <translation>createEllipse(x, y, width, height, [&quot;name&quot;]) -&gt; строкова величина

Створює новий еліпс на активній сторінці та повертає його назву.
Координати задаються в активних одиницях виміру документа (див.
константи UNIT). &quot;name&quot; повинне бути неповторним визначником
об&apos;єкта, тому що воно служить для подальших звернень до нього.
Якщо &quot;name&quot; не задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
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
        <translation>createImage(x, y, width, height, [&quot;name&quot;]) -&gt; строкова величина

Створює нову рамку для зображень на активній сторінці та повертає
її назву. Координати задаються в активних одиницях виміру документа.
&quot;name&quot; повинне бути неповторним визначником об&apos;єкта, тому що воно 
служить для подальших звернень до нього. Якщо &quot;name&quot; не задано, 
Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
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
        <translation>createText(x, y, width, height, [&quot;name&quot;]) -&gt; строкова величина

Створює нову текстову рамку на активній сторінці та повертає
її назву. Координати задаються в активних одиницях виміру документа
(див константи UNIT). &quot;name&quot; повинне бути неповторним визначником 
об&apos;єкта, тому що воно служить для подальших звернень до нього. Якщо 
&quot;name&quot; не задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.

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
        <translation>createLine(x1, y1, x2, y2, [&quot;name&quot;]) -&gt; строкова величина

Створює нову лінію від точки (x1, y1) до точки (x2, y2) та повертає
її назву. Координати задаються в активних одиницях виміру документа
(див константи UNIT). &quot;name&quot; повинне бути неповторним визначником 
об&apos;єкта, тому що воно служить для подальших звернень до нього. Якщо 
&quot;name&quot; не задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
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
        <translation>createPolyLine(list, [&quot;name&quot;]) -&gt; строкова величина

Створює багатосегментну лінію та повертає її назву. Точки лінії 
зберігаються в списку &quot;list&quot; у визначеному порядку [x1, y1, x2, y2...xn. yn]. 
Координати задаються в активних одиницях виміру документа
(див константи UNIT). &quot;name&quot; повинне бути неповторним визначником 
об&apos;єкта, тому що воно служить для подальших звернень до нього. Якщо 
&quot;name&quot; не задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
Може повернути помилку ValueError, якщо задане недостатнє число точок
або число заданих координат не відповідає числу точок.
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
        <translation>createPolygon(list, [&quot;name&quot;]) -&gt; строкова величина

Створює полігон та повертає його назву. Точки кутів полігона
зберігаються в списку &quot;list&quot; у визначеному порядку [x1, y1, x2, y2...xn. yn]. 
Як мінімум мають бути задані три точки. Необхідності в повторенні першої
точки для замикання полігона не існує. Полігон автоматично замикається
з&apos;єднанням першої та останньої заданих точок. Координати задаються в 
активних одиницях виміру документа (див константи UNIT). &quot;name&quot; повинне 
бути неповторним визначником об&apos;єкта, тому що воно служить для подальших 
звернень до нього. Якщо &quot;name&quot; не задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
Може повернути помилку ValueError, якщо задане недостатнє число точок
або число заданих координат не відповідає числу точок.
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
        <translation>createBezierLine(list, [&quot;name&quot;]) -&gt; строкова величина

Створює нову криву Безьє  та повертає її назву. Точки кривої
зберігаються в списку &quot;list&quot; у визначеному порядку 
[x1, y1, kx1, ky1, x2, y2, kx2, ky2...xn. yn, kxn. kyn]
В списку точок x та y відповідають координатам точки, а kx та ky 
відповідають контрольним точкам кривої. Координати задаються 
в активних одиницях виміру документа (див константи UNIT). 
&quot;name&quot; повинне бути неповторним визначником об&apos;єкта, тому що 
воно служить для подальших звернень до нього. Якщо &quot;name&quot; не 
задано, Scribus створить його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
Може повернути помилку ValueError, якщо задане недостатнє число точок
або число заданих координат не відповідає числу точок.
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
        <translation>createPathText(x, y, &quot;textbox&quot;, &quot;beziercurve&quot;, [&quot;name&quot;]) -&gt; строкова величина

Створює новий текстовий шлях (pathText) через з&apos;єднання двох об&apos;єктів
&quot;textbox&quot; та &quot;beziercurve&quot; та повертає його назву. Координати задаються 
в активних одиницях виміру документа (див константи UNIT). &quot;name&quot; 
повинне бути неповторним визначником об&apos;єкта, тому що воно служить 
для подальших звернень до нього. Якщо &quot;name&quot; не задано, Scribus створить 
його сам.

Може повернути помилку NameExistsError при спробі задання вже
використаної назви.
Може повернути помилку NotFoundError, якщо один чи обидва названих
базових об&apos;єкти не існують.
</translation>
    </message>
    <message>
        <source>deleteObject([&quot;name&quot;])

Deletes the item with the name &quot;name&quot;. If &quot;name&quot; is not given the currently
selected item is deleted.
</source>
        <translation>deleteObject([&quot;name&quot;])

Видаляє об&apos;єкт &quot;name&quot;. Якщо &quot;name&quot; не вказано, буде видалено вибраний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>textFlowsAroundFrame(&quot;name&quot; [, state])

Enables/disables &quot;Text Flows Around Frame&quot; feature for object &quot;name&quot;.
Called with parameters string name and optional boolean &quot;state&quot;. If &quot;state&quot;
is not passed, text flow is toggled.
</source>
        <translation>textFlowsAroundFrame(&quot;name&quot; [, state])

Вмикає/вимикає параметр &quot;Текст обтікає рамку&quot; об&apos;єкта &quot;name&quot;.
При виклиці задається строковий параметр &quot;name&quot; та необов&apos;язковий
булевий параметр &quot;state&quot;. Якщо &quot;state&quot; не задано, відбувається зміна
стану параметра на протилежний.
</translation>
    </message>
    <message>
        <source>objectExists([&quot;name&quot;]) -&gt; bool

Test if an object with specified name really exists in the document.
The optional parameter is the object name. When no object name is given,
returns True if there is something selected.
</source>
        <translation>objectExists([&quot;name&quot;]) -&gt; булеве значення

Перевіряє чи вказаний об&apos;єкт вже існує в документі. Назва об&apos;єкта є 
необов&apos;язковим параметром. Якщо назва не задана, повертає булеве
значення &quot;True&quot;, при наявності вибраного об&apos;єкта.
</translation>
    </message>
    <message>
        <source>setStyle(&quot;style&quot; [, &quot;name&quot;])

Apply the named &quot;style&quot; to the object named &quot;name&quot;. If is no object name
given, it&apos;s applied on the selected object.
</source>
        <translation>setStyle(&quot;style&quot; [, &quot;name&quot;])

Застосовує стиль &quot;style&quot; для об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; не вказано, 
буде використано вибраний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>getAllStyles() -&gt; list

Return a list of the names of all paragraph styles in the current document.
</source>
        <translation>getAllStyles() -&gt; список

Повертає список всіх стилів абзаців в активному документі.
</translation>
    </message>
    <message>
        <source>currentPage() -&gt; integer

Returns the number of the current working page. Page numbers are counted from 1
upwards, no matter what the displayed first page number of your document is.
</source>
        <translation>currentPage() -&gt; ціле число

Повертає номер активної сторінки. Номери сторінок рахуються вгору від 1,
незалежно від того, який номер показаний на першій сторінці документа.
</translation>
    </message>
    <message>
        <source>redrawAll()

Redraws all pages.
</source>
        <translation>redrawAll()

Перерисовує всі сторінки.
</translation>
    </message>
    <message>
        <source>savePageAsEPS(&quot;name&quot;)

Saves the current page as an EPS to the file &quot;name&quot;.

May raise ScribusError if the save failed.
</source>
        <translation>savePageAsEPS(&quot;name&quot;)

Зберігає активну сторінку, як файл EPS з назвою &quot;name&quot;.

Може повернути помилку ScribusError, якщо запис не був успішним.
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

Видалає дану сторінку. Якщо в документі є лише одна сторінка - нічого
не відбудеться. Номери сторінок рахуються вгору від 1, незалежно від 
того, який номер показаний на першій сторінці документа.

Може повернути помилку IndexError, якщо номер сторінки виходить за
межі доступного діапазону
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

Переходить до сторінки номер &quot;nr&quot; (тобто робить сторінку номер &quot;nr&quot; активною). 
Майте на увазі, що gotoPage лише установлює сторінку на якій буде оперувати
сценарій і не змінює того, яка сторінка видима користувачу.

Може повернути помилку IndexError, якщо номер сторінки виходить за межі 
доступного діапазону.
</translation>
    </message>
    <message>
        <source>pageCount() -&gt; integer

Returns the number of pages in the document.
</source>
        <translation>pageCount() -&gt; ціле число

Повертає число сторінок в документі.
</translation>
    </message>
    <message>
        <source>getHGuides() -&gt; list

Returns a list containing positions of the horizontal guides. Values are in the
document&apos;s current units - see UNIT_&lt;type&gt; constants.
</source>
        <translation>getHGuides() -&gt; список

Повертає список розміщення горизонтальних направляючих. Значення
виражаються в активних одиницях виміру документа - див. константи
UNIT_&lt;type&gt;.
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

Установлює горизонтальні направляючі. Вхідний параметр має бути заданий
як список позицій направляючих, виражених в активних одиницях виміру
документа - див. константи UNIT_&lt;type&gt;.

Приклад: setHGuides(getHGuides() + [200.0, 210.0] # додати нові направляючі без
заміщення існуючих
         setHGuides([90,250]) # повністю замінити існуючі направляючі
</translation>
    </message>
    <message>
        <source>getVGuides()

See getHGuides.
</source>
        <translation>getVGuides()

Див. getHGuides.
</translation>
    </message>
    <message>
        <source>setVGuides()

See setHGuides.
</source>
        <translation>setVGuides()

Див. setHGuides.
</translation>
    </message>
    <message>
        <source>getPageSize() -&gt; tuple

Returns a tuple with page dimensions measured in the document&apos;s current units.
See UNIT_&lt;type&gt; constants and getPageMargins()
</source>
        <translation>getPageSize() -&gt; тупль

Повертає тупль з розміром сторінки, вираженим в активних одиницях виміру
документа. Див. константи UNIT_&lt;type&gt; та функцію getPageMargins()
</translation>
    </message>
    <message>
        <source>getPageItems() -&gt; list

Returns a list of tuples with items on the current page. The tuple is:
(name, objectType, order) E.g. [(&apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
means that object named &apos;Text1&apos; is a text frame (type 4) and is the first at
the page...
</source>
        <translation>getPageItems() -&gt; список

Повертає список туплів з об&apos;єктами, розташованими на активній сторінці.
Тупль має вигляд (name, objectType, order).  Наприклад &apos;Text1&apos;, 4, 0), (&apos;Image1&apos;, 2, 1)]
означає, що об&apos;єкт &apos;Text1&apos; є текстовою рамкою (тип 4) і є першим об&apos;єктом сторінки...
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (left, right, top, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation type="obsolete">getPageMargins()

Повертає поля сторінки у вигляді тупля (ліве, праве, верхнє, нижнє) в
активних одиницях виміру. Див. константи UNIT_&lt;type&gt; та функцію getPageSize().
</translation>
    </message>
    <message>
        <source>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Sets the fill color of the object &quot;name&quot; to the color &quot;color&quot;. &quot;color&quot;
is the name of one of the defined colors. If &quot;name&quot; is not given the
currently selected item is used.
</source>
        <translation>setFillColor(&quot;color&quot;, [&quot;name&quot;])

Установлює колір заповнення &quot;color&quot; для об&apos;&apos;єкта &quot;name&quot;. &quot;color&quot;
повинен бути попередньо визначеним. Якщо &quot;name&quot; не задано, 
буде використано активний об&apos;єкт.
</translation>
    </message>
    <message>
        <source>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Sets the line color of the object &quot;name&quot; to the color &quot;color&quot;. If &quot;name&quot;
is not given the currently selected item is used.
</source>
        <translation>setLineColor(&quot;color&quot;, [&quot;name&quot;])

Установлює колір &quot;color&quot; для об&apos;&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; не задано, 
буде використано активний об&apos;єкт.
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

Установлює товщину об&apos;єкта &quot;name&quot; до &quot;width&quot;. &quot;width&quot; повинна бути в
діапазоні від 0.0 до 12.0 включно і вимірюється в точках. Якщо &quot;name&quot; 
не задано, буде використано активний об&apos;єкт. 

Може повернути помилку ValueError, якщо товщина лінії виходить за
доступний діапазон.
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

Установлює тінь ліній об&apos;єкта &quot;name&quot; до &quot;shade&quot;. &quot;shade&quot; повинна бути 
в діапазоні від 0 (найсвітліша) до 100 (повна насиченість кольору).
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт. 

Може повернути помилку ValueError, якщо тінь виходить за 
доступний діапазон.
</translation>
    </message>
    <message>
        <source>setLineJoin(join, [&quot;name&quot;])

Sets the line join style of the object &quot;name&quot; to the style &quot;join&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for join - JOIN_&lt;type&gt;.
</source>
        <translation>setLineJoin(join, [&quot;name&quot;])

Установлює тип з&apos;єднань ліній об&apos;єкта &quot;name&quot; до стиля &quot;join&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
Існують попередньо визначені константи для &quot;join&quot; - JOIN_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineEnd(endtype, [&quot;name&quot;])

Sets the line cap style of the object &quot;name&quot; to the style &quot;cap&quot;.
If &quot;name&quot; is not given the currently selected item is used. There are
predefined constants for &quot;cap&quot; - CAP_&lt;type&gt;.
</source>
        <translation>setLineEnd(endtype, [&quot;name&quot;])

Установлює тип кінцівок ліній об&apos;єкта &quot;name&quot; до стиля &quot;cap&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
Існують попередньо визначені константи для &quot;cap&quot; - CAP_&lt;type&gt;.
</translation>
    </message>
    <message>
        <source>setLineStyle(style, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the style &quot;style&quot;. If &quot;name&quot;
is not given the currently selected item is used. There are predefined
constants for &quot;style&quot; - LINE_&lt;style&gt;.
</source>
        <translation>setLineStyle(style, [&quot;name&quot;])

Установлює стиль ліній об&apos;єкта &quot;name&quot; до стиля &quot;style&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.
Існують попередньо визначені константи для &quot;style&quot; - LINE_&lt;style&gt;.
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

Установлює тінь заповнення об&apos;єкта &quot;name&quot; до &quot;shade&quot;. &quot;shade&quot; 
повинна бути в діапазоні від 0 (найсвітліша) до 100 (повна 
насиченість кольору). Якщо &quot;name&quot; не задано, буде використано 
активний об&apos;єкт. 

Може повернути помилку ValueError, якщо тінь виходить за 
доступний діапазон.
</translation>
    </message>
    <message>
        <source>setCornerRadius(radius, [&quot;name&quot;])

Sets the corner radius of the object &quot;name&quot;. The radius is expressed
in points. If &quot;name&quot; is not given the currently selected item is used.

May raise ValueError if the corner radius is negative.
</source>
        <translation>setCornerRadius(radius, [&quot;name&quot;])

Установлює радіус кутів об&apos;єкта &quot;name&quot;. Радіус виражається в точках.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт. 

Може повернути помилку ValueError, якщо радіус негативний.
</translation>
    </message>
    <message>
        <source>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Sets the line style of the object &quot;name&quot; to the named style &quot;namedStyle&quot;.
If &quot;name&quot; is not given the currently selected item is used.

May raise NotFoundError if the line style doesn&apos;t exist.
</source>
        <translation>setMultiLine(&quot;namedStyle&quot;, [&quot;name&quot;])

Установлює стиль ліній об&apos;єкта &quot;name&quot; до стиля &quot;namedStyle&quot;.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку NotFoundError, якщо цей стиль ліній не існує.
</translation>
    </message>
    <message>
        <source>progressReset()

Cleans up the Scribus progress bar previous settings. It is called before the
new progress bar use. See progressSet.
</source>
        <translation>progressReset()

Видалає попередні установки панелі прогресу. Викликається до нового
використання панелі. Див. progressSet.
</translation>
    </message>
    <message>
        <source>progressTotal(max)

Sets the progress bar&apos;s maximum steps value to the specified number.
See progressSet.
</source>
        <translation>progressTotal(max)

Установлює максимальне значення для панелі прогресу до заданого
число. Див. progressSet.
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

Установлює позицію панелі прогресу до значення &quot;nr&quot; відносно 
попередньо вираженого значення progressTotal. Панель прогресу
виражає концепцію сходинок - задається загальне число сходинок
та число пройдених сходинок і панель буде показувати прогрес
проходження сходинок у відсотках від завершення. Сумма сходинок
задається значенням progressTotal(). Активне число сходинок
установлюється за допомогою progressSet(). Панель прогресу може 
бути повернена на початок за допомогою progressReset(). [основано
на документації фірми Trolltech&apos;s по Qt]
</translation>
    </message>
    <message>
        <source>setCursor()

[UNSUPPORTED!] This might break things, so steer clear for now.
</source>
        <translation>setCursor()

[НЕ ПІДТРИМУЄТЬСЯ!] Може призвести до помилок в роботі програми.
Поки що не використовуйте.
</translation>
    </message>
    <message>
        <source>docChanged(bool)

Enable/disable save icon in the Scribus icon bar and the Save menu item. It&apos;s
useful to call this procedure when you&apos;re changing the document, because Scribus
won&apos;t automatically notice when you change the document using a script.
</source>
        <translation>docChanged(булеве значення)

Ввімкнути/вимкнути іконку запису на панелі іконокScribus та в меню &quot;Записати&quot;.
Використовуйте цю функцію при зміні документа, тому що Scribus не реєструє
змін автоматично при роботі сценарія.
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

Визначає новий колір &quot;name&quot;. Значення кольору виражається чотирма
складовими: c = Cyan (Салатовий), m = Magenta (Малиновий), 
y = Yellow (Жовтий) та k = Black (Чорний). Складові кольору повинні
бути в діапазоні від 0 до 255.

Може повернути помилку ValueError, якщо була задана недійсна
назва кольору.
</translation>
    </message>
    <message>
        <source>getCornerRadius([&quot;name&quot;]) -&gt; integer

Returns the corner radius of the object &quot;name&quot;. The radius is
expressed in points. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">getCornerRadius([&quot;name&quot;]) -&gt; ціле число

Повертає радіус кутів об&apos;єкта &quot;name&quot;. Радіус виражається в точках.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.</translation>
    </message>
    <message>
        <source>getPosition([&quot;name&quot;]) -&gt; (x,y)

Returns a (x, y) tuple with the position of the object &quot;name&quot;.
If &quot;name&quot; is not given the currently selected item is used.
The position is expressed in the actual measurement unit of the document
- see UNIT_&lt;type&gt; for reference.
</source>
        <translation type="obsolete">getPosition([&quot;name&quot;]) -&gt; (x,y)

Повертає тупль (x, y) з позицією об&apos;єкта &quot;name&quot;. Якщо &quot;name&quot; 
не задано, буде використано активний об&apos;єкт. Позиція виражається
а активних точках виміру документа - див. UNIT_&lt;type&gt; для 
довідки.
</translation>
    </message>
    <message>
        <source>rotateObjectAbs(rot [, &quot;name&quot;])

Sets the rotation of the object &quot;name&quot; to &quot;rot&quot;. Positive values
mean counter clockwise rotation. If &quot;name&quot; is not given the currently
selected item is used.
</source>
        <translation type="obsolete">rotateObjectAbs(rot [, &quot;name&quot;])

Установлює кут повороту об&apos;єкта &quot;name&quot; до &quot;rot&quot;. Позитивні
значення приводять до обертання проти годинникової стрілки.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт. 
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

Установлює масштабування зображення до розмірів рамки для вибраної
або вказаної рамки для зображень до `scaletoframe&apos;. Вибір
`proportional&apos; (пропорційне) значення встановлює фіксоване відношення 
сторін зображення при масштабуванні. І `scaletoframe&apos; і `proportional&apos; являються
булевими змінними.

Може повернути помилку WrongFrameTypeError.
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

Виділяє &quot;count&quot; знаків тексту в текстовій рамці &quot;name&quot; починаючи
з знака &quot;start&quot;. Рахунок знаків починається з 0. Якщо &quot;count&quot; 
дорівнює нулю вибірка тексту буде відмінена. Якщо &quot;count&quot; дорівнює
 -1 - весь текст в рамці буде вибрано. Якщо &quot;name&quot; не задано буде
використано ім&apos;я активного об&apos;єкта.

Може повернути помилку IndexError, якщо вибірка виходить за допустимий
діапазон тексту.
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

Створити макрос з іменем &quot;name&quot; методом оцінки програмного коду 
&quot;sourcetext&quot;. &quot;sourcetext&quot;  код повинен відповідати тим же вимогам,
що і макроси створені графічною оболонкою. Якщо задана, строкова
величина &quot;accel&quot; то вона буде використана як гаряча комбінація
клавіш для цього макроса.
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

Повертає булеве значення, яке вказує на те чи можна друкувати план &quot;layer&quot;.
True означає, що &quot;layer&quot; можна друкувати, False означає, що друк плана
&quot;layer&quot; не дозволено.

Може повернути помилку NotFoundError, якщо план не знайдено.
May raise ValueError при використанні недопустимої назви плана.
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

Повертає тупль (R,G,B), який містить три компоненти кольору з
назвою &quot;name&quot; з активного документа, конвертовані в область
кольору RGB. При відсутності відчиненого документа повертає
значення названого кольору з стандартної палітри документа.

Може повернути помилку NotFoundError, якщо названий колір
не було знайдено.
Може повернути помилку ValueError, якщо було вказане невірне
ім&apos;я кольору.
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

Створює растрове зображення шрифта з іменем &quot;name&quot; з заданим
текстом &quot;sample&quot; та розміром &quot;size&quot;. Якщо &quot;filename&quot; не дорівнює &quot;&quot;, 
зображення буде записане в файл з назвою &quot;filename&quot;. В протилежному
випадку зображення буде повернене, як строкова величина.
Необов&apos;язковий аргумент &quot;format&quot; вказує на те, який формат зображення
генерувати і підтримує всі формати дозволені QPixmap.save(). Часто
вживані формати включають PPM, JPEG, PNG та XPM.

Може повернути помилку NotFoundError, якщо вказаний шрифт не
було знайдено.
Може повернути помилку ValueError, якщо функції був переданий пустий
зразковий текст або ім&apos;я файла.
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

Повертає значення видимості плана &quot;layer&quot; - True означає, що
план &quot;layer&quot; видимий, а False означає, що план &quot;layer&quot; невидимий.

Може повернути помилку NotFoundError, якщо план не знайдено.
Може повернути помилку ValueError, якщо назва плана не допустима.
</translation>
    </message>
    <message>
        <source>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Sets wether (toggle = 1) the text frame &quot;name&quot; is a bookmark nor not.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>setPDFBookmark(&quot;toggle&quot;, [&quot;name&quot;])

Установлює те чи (&quot;toggle&quot; = 1) текстова рамка &quot;name&quot; є закладкою.
Якщо ім&apos;я &quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку WrongFrameTypeError, якщо задана рамка не є 
текстовою рамкою
</translation>
    </message>
    <message>
        <source>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Returns true if the text frame &quot;name&quot; is a PDF bookmark.
If &quot;name&quot; is not given the currently selected item is used.

May raise WrongFrameTypeError if the target frame is not a text frame
</source>
        <translation>isPDFBookmark([&quot;name&quot;]) -&gt; bool

Повертає булеве значення true, якщо текстова рамка &quot;name&quot; є закладкою PDF.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку WrongFrameTypeError, якщо задана рамка не
являється текстовою
</translation>
    </message>
    <message>
        <source>getPageMargins()

Returns the page margins as a (top, left, right, bottom) tuple in the current
units. See UNIT_&lt;type&gt; constants and getPageSize().
</source>
        <translation>getPageMargins()

Повертає поля сторінки як тупль (верхнє, ліве, праве, нижнє) в активних одиницях. Дивіться UNIT_&lt;type&gt; константи та getPageSize().
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

Вставляє текст &quot;text&quot; в позицію &quot;pos&quot; в текстовій рамці &quot;name&quot;.
Текст повинен бути в кодуванні UTF (див. setText() для довідки). Перший
знак повинен мати індекс 0. Вставка в позиції -1 добавляє текст в рамку.
Якщо &quot;name&quot; не задано, буде використано активний об&apos;єкт.

Може повернути помилку IndexError при вставці за межами доступного
діапазону.
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
        <source>textOverflows([&quot;name&quot;, nolinks]) -&gt; integer

Returns the actual number of overflowing characters in text frame &quot;name&quot;.
If is nolinks set to non zero value it takes only one frame - it doesn&apos;t
use text frame linking. Without this parameter it search all linking chain.

May raise WrongFrameTypeError if the target frame is not an text frame
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>About</name>
    <message>
        <source>About Scribus%1%2</source>
        <translation type="obsolete">Про Scribus %1 %2</translation>
    </message>
    <message>
        <source>%1. %2 %3 </source>
        <translation type="obsolete">%1. %2 %3</translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3</source>
        <translation type="obsolete">Версія Scribus %1
%2 %3</translation>
    </message>
    <message>
        <source>Build-ID:</source>
        <translation> Збірка:</translation>
    </message>
    <message>
        <source>Contributions from:</source>
        <translation>У співпраці з:</translation>
    </message>
    <message>
        <source>Windows port:</source>
        <translation type="obsolete">Портування до Віндоус:</translation>
    </message>
    <message>
        <source>German:</source>
        <translation type="obsolete">Німецька:</translation>
    </message>
    <message>
        <source>French:</source>
        <translation type="obsolete">Французська:</translation>
    </message>
    <message>
        <source>Italian:</source>
        <translation type="obsolete">Італійська:</translation>
    </message>
    <message>
        <source>Hungarian:</source>
        <translation type="obsolete">Венгерська:</translation>
    </message>
    <message>
        <source>Ukrainian:</source>
        <translation type="obsolete">Українська:</translation>
    </message>
    <message>
        <source>Bulgarian:</source>
        <translation type="obsolete">Болгарська:</translation>
    </message>
    <message>
        <source>Galician:</source>
        <translation type="obsolete">Галицька:</translation>
    </message>
    <message>
        <source>Turkish:</source>
        <translation type="obsolete">Турецька:</translation>
    </message>
    <message>
        <source>Lithuanian:</source>
        <translation type="obsolete">Литовська:</translation>
    </message>
    <message>
        <source>Polish:</source>
        <translation type="obsolete">Польська:</translation>
    </message>
    <message>
        <source>Czech:</source>
        <translation type="obsolete">Чешська:</translation>
    </message>
    <message>
        <source>Slovak:</source>
        <translation type="obsolete">Словацька:</translation>
    </message>
    <message>
        <source>Danish:</source>
        <translation type="obsolete">Датська:</translation>
    </message>
    <message>
        <source>Norwegian:</source>
        <translation type="obsolete">Норвежська:</translation>
    </message>
    <message>
        <source>Welsh:</source>
        <translation type="obsolete">Уельська:</translation>
    </message>
    <message>
        <source>Russian:</source>
        <translation type="obsolete">Російська:</translation>
    </message>
    <message>
        <source>Brazilian:</source>
        <translation type="obsolete">Бразільська:</translation>
    </message>
    <message>
        <source>Finnish:</source>
        <translation type="obsolete">Фінська:</translation>
    </message>
    <message>
        <source>Basque:</source>
        <translation type="obsolete">Баскська:</translation>
    </message>
    <message>
        <source>Slovenian:</source>
        <translation type="obsolete">Словенська:</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Ця панель показує версію, дату збірки і 
використання зовнішніх бібліотек в Scribus.
С-С-T можна розібрати як С=CUPS - Загальна 
система друку для Юнікс, С=littlecms - Проста
система керування кольором, T=TIFF - Підтримка
формату зображень TIFF. При відсутності бібліотеки
літера індикатор заміщається на *</translation>
    </message>
    <message>
        <source>&amp;About</source>
        <translation>&amp;Про</translation>
    </message>
    <message>
        <source>A&amp;uthors</source>
        <translation>&amp;Автори</translation>
    </message>
    <message>
        <source>&amp;Translations</source>
        <translation>&amp;Перекладачі</translation>
    </message>
    <message>
        <source>&amp;Online</source>
        <translation>&amp;Веб ресурси</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Зачинити</translation>
    </message>
    <message>
        <source>Development Team:</source>
        <translation>Команда розробників:</translation>
    </message>
    <message>
        <source>Official Documentation:</source>
        <translation>Офіційна документація:</translation>
    </message>
    <message>
        <source>Other Documentation:</source>
        <translation>Інша документація:</translation>
    </message>
    <message>
        <source>English (British):</source>
        <translation type="obsolete">Англійська (Британська):</translation>
    </message>
    <message>
        <source>Swedish:</source>
        <translation type="obsolete">Шведська:</translation>
    </message>
    <message>
        <source>Homepage</source>
        <translation>Домашня сторінка</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation>Документація на Інтернеті</translation>
    </message>
    <message>
        <source>Bugs and Feature Requests</source>
        <translation>Звітування про помилки в програмі та заявки на нові можливості</translation>
    </message>
    <message>
        <source>Mailing List</source>
        <translation>Список розсилки</translation>
    </message>
    <message>
        <source>Official Translations and Translators:</source>
        <translation>Офіційні переклади та перекладачі:</translation>
    </message>
    <message>
        <source>Esperanto:</source>
        <translation type="obsolete">Есперанто:</translation>
    </message>
    <message>
        <source>Korean:</source>
        <translation type="obsolete">Корейська:</translation>
    </message>
    <message>
        <source>Serbian:</source>
        <translation type="obsolete">Сербська:</translation>
    </message>
    <message>
        <source>Spanish:</source>
        <translation type="obsolete">Іспанська:</translation>
    </message>
    <message>
        <source>Previous Translation Contributors:</source>
        <translation>Попередні учасники перекладання:</translation>
    </message>
    <message>
        <source>Catalan:</source>
        <translation type="obsolete">Каталанська:</translation>
    </message>
    <message>
        <source>About Scribus %1</source>
        <translation>Про Scribus %1</translation>
    </message>
    <message>
        <source>This panel shows the version, build date and
 compiled in library support in Scribus
The C-C-T equates to C=littlecms C=CUPS T=TIFF support.
Missing library support is indicated by a *</source>
        <translation type="obsolete">Ця панель показує версію, дату збірки та
вбудовану підтримку бібліотек в Scribus.
C-C-T означає C=littlecms C=CUPS T=TIFF
підтримку. Відсутність певної бібліотеки
вказується зірочкою (*)
littlecms = бібліотека профілювання кольорів
CUPS = бібліотека підтримки системи друку
TIFF = бібліотека підтримки графічного формату
tiff</translation>
    </message>
    <message>
        <source>%1 %2 %3 </source>
        <translation>%1 %2 %3 </translation>
    </message>
    <message>
        <source>Afrikaans:</source>
        <translation type="obsolete">Африкаанс:</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian):</source>
        <translation type="obsolete">Португальська (Бразильська):</translation>
    </message>
    <message>
        <source>Using GhostScript version %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>No GS version available</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus Version %1
%2 %3 (%4)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Mac OSX Aqua Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Windows Port:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This panel shows the version, build date and compiled in library support in Scribus. The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support. Last Letter is the renderer C=cairo or A=libart Missing library support is indicated by a * This also indicates the version of Ghostscript which Scribus has detected.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AboutPlugins</name>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Так</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Ні</translation>
    </message>
</context>
<context>
    <name>AboutPluginsBase</name>
    <message>
        <source>Scribus: About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>File Name:</source>
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
        <source>Copyright:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Author(s):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Description:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>License:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Зачинити</translation>
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
        <translation type="unfinished">&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Відчинити...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Зачинити</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Записати</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Записати &amp;як...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="unfinished">Повернутися до за&amp;писаного</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="unfinished">Зібрати для в&amp;иводу...</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="unfinished">Вставити текст...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="unfinished">Додати &amp;текст...</translation>
    </message>
    <message>
        <source>Get Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="unfinished">Записати &amp;текст...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="unfinished">Записати, як &amp;ЕPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="unfinished">Записати, як &amp;РDF...</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="unfinished">Установка &amp;опцій документу...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="unfinished">&amp;Друк...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="unfinished">&amp;Вихід</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="unfinished">&amp;Відмінити</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Повторити</translation>
    </message>
    <message>
        <source>&amp;Item Action Mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Вклеїти</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="unfinished">Очистити &amp;вміст</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="unfinished">Виді&amp;лити все</translation>
    </message>
    <message>
        <source>&amp;Deselect All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="unfinished">&amp;Пошук/Заміна...</translation>
    </message>
    <message>
        <source>Edit Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="unfinished">&amp;Кольори...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="unfinished">&amp;Стилі абзаців...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="unfinished">Стилі &amp;ліній...</translation>
    </message>
    <message>
        <source>&amp;Master Pages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">С&amp;ценарії Javascript...</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="unfinished">&amp;Установки...</translation>
    </message>
    <message>
        <source>%1 pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="unfinished">&amp;Інше...</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="unfinished">&amp;Ліве</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="unfinished">По &amp;центру</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="unfinished">&amp;Праве</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="unfinished">&amp;Блок</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="unfinished">&amp;Вимушене</translation>
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
        <translation type="unfinished">&amp;Підкреслення</translation>
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
        <translation type="unfinished">С&amp;хема документу</translation>
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
        <translation type="unfinished">&amp;Табулятори...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="unfinished">&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="unfinished">&amp;Багаторазове дублювання</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="unfinished">З&amp;групувати</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="unfinished">&amp;Розгрупувати</translation>
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
        <translation type="unfinished">&amp;Опустити</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="unfinished">&amp;Підняти</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="unfinished">Послат в &amp;чорновик</translation>
    </message>
    <message>
        <source>&amp;Attributes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="unfinished">&amp;Показувати зображення</translation>
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
        <translation type="unfinished">&amp;Закладка PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="unfinished">&amp;Аннотація PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="unfinished">В&amp;ластивості аннотації</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="unfinished">Властивості &amp;поля</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation type="unfinished">&amp;Редагувати форму...</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="unfinished">При&amp;єднати текст до шляху</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="unfinished">В&amp;ідєднати текст від шляху</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="unfinished">Поєдн&amp;ати полігони</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="unfinished">Розділ&amp;ити полігони</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="unfinished">Крива &amp;Безьє</translation>
    </message>
    <message>
        <source>&amp;Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="unfinished">&amp;Контури</translation>
    </message>
    <message>
        <source>&amp;Polygon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="unfinished">&amp;Текстова рамка</translation>
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
        <translation type="unfinished">&amp;Вставити...</translation>
    </message>
    <message>
        <source>Im&amp;port...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="unfinished">&amp;Видалити...</translation>
    </message>
    <message>
        <source>&amp;Copy...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="unfinished">&amp;Перемістити...</translation>
    </message>
    <message>
        <source>&amp;Apply Master Page...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="unfinished">Установка роз&amp;мітки...</translation>
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
        <translation type="unfinished">&amp;Мініатюрні зображення</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="unfinished">Показати &amp;розмітку полів</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="unfinished">Показати ра&amp;мки</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="unfinished">&amp;Показати зображення</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="unfinished">Показати &amp;сітку</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="unfinished">Показати розм&amp;ітку</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="unfinished">Показати &amp;базову сітку</translation>
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
        <translation type="unfinished">П&amp;ритягування до сітки</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="unfinished">Притягування до розмітк&amp;и</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="unfinished">&amp;Властивості</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="unfinished">&amp;Чорновик</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="unfinished">П&amp;лани</translation>
    </message>
    <message>
        <source>&amp;Arrange Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="unfinished">&amp;Закладки</translation>
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
        <translation type="unfinished">&amp;Інструменти</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="unfinished">PDF &amp;Інструменти</translation>
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
        <translation type="unfinished">&amp;Фігура</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation type="unfinished">&amp;Лінія</translation>
    </message>
    <message>
        <source>&amp;Freehand Line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="unfinished">Повернути об&apos;єкт</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="unfinished">Змінити масштаб зображення</translation>
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
        <translation type="unfinished">Редагувати зміст рамки</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="unfinished">Редагувати текст...</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="unfinished">Установити зв&apos;язок між текстовими рамками</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="unfinished">Розірвати зв&apos;язок між текстовими рамками</translation>
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
        <translation type="unfinished">Редагувати текст вбудованим редактором</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="unfinished">Вставити текстову рамку</translation>
    </message>
    <message>
        <source>Insert Image Frame</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="unfinished">Вставити таблицю</translation>
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
        <translation type="unfinished">Вставити вільно рисовану лінію</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="unfinished">Керування &amp;зображеннями</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="unfinished">Пе&amp;ренос тексту</translation>
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
        <translation type="unfinished">&amp;Про програму Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="unfinished">Про &amp;систему Qt</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="unfinished">Пі&amp;дказки</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="unfinished">&amp;Довідка по Scribus...</translation>
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
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="unfinished">Перегляд перед д&amp;руком</translation>
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
        <translation type="unfinished">&amp;Каскадне розміщення</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="unfinished">&amp;Мозаїчне розміщення</translation>
    </message>
    <message>
        <source>&amp;About Plug-ins</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AdvOptions</name>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Додаткові установки</translation>
    </message>
    <message>
        <source>Creates PostScript Level 3</source>
        <translation type="obsolete">Створює постскрипт 3 рівня</translation>
    </message>
    <message>
        <source>Creates PostScript Level 2 only, beware,
this can create huge files</source>
        <translation type="obsolete">Створює постскрипт 2 рівня. Можливе 
значне збільшення розміру файла</translation>
    </message>
    <message>
        <source>Creates PostScript Level 1 only, beware,
this can create huge files</source>
        <translation type="obsolete">Створює постскрипт 1 рівня. Можливе 
значне збільшення розміру файла</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Horizontal</source>
        <translation type="obsolete">Віддзеркалити сторінку(и) &amp;горизонтально</translation>
    </message>
    <message>
        <source>Mirror Page(s) &amp;Vertical</source>
        <translation type="obsolete">Віддзеркалити сторінку(и) &amp;вертикально</translation>
    </message>
    <message>
        <source>Apply &amp;ICC Profiles</source>
        <translation type="obsolete">Застосувати &amp;ICC профілі</translation>
    </message>
    <message>
        <source>PostScript Level &amp;1</source>
        <translation type="obsolete">&amp;1 рівень постскрипту</translation>
    </message>
    <message>
        <source>PostScript Level &amp;2</source>
        <translation type="obsolete">&amp;2 рівень постскрипту</translation>
    </message>
    <message>
        <source>PostScript Level &amp;3</source>
        <translation type="obsolete">&amp;3 рівень постскрипту</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Apply Under Color &amp;Removal</source>
        <translation type="obsolete">Виконати видалення &amp;кольорів</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Метод переводу певних відтінків сірого, які складаються з салатового, 
жовтого та рожевого кольорів, на використання чорного кольору.
UCR найчастіше впливає на ті частини зображень, які складаються з нейтральних
та/чи темних відтінків близьких до сірого. Використання цієї установки може
покращити якість друку деяких зображень. Деяке тестування та експерименти
необхідні в кожному конкретному випадку. UCR знижує ймовірність перенасичення
паперу CMY чорнилами.</translation>
    </message>
    <message>
        <source>Set Media Size</source>
        <translation type="obsolete">Установити розмір паперу</translation>
    </message>
    <message>
        <source>This enables you to explicitely set,
the media size of the postscript file.
Not recommended unless
 requested by your printer.</source>
        <translation type="obsolete">Дозволяє Вам явно установити
розмір паперу постскрипт файлу.
Не рекомендується, якщо не
вимагається вашим друкарем.</translation>
    </message>
</context>
<context>
    <name>Align</name>
    <message>
        <source>Distribute/Align</source>
        <translation type="obsolete">Розташувати/Вирівняти</translation>
    </message>
    <message>
        <source>Align</source>
        <translation type="obsolete">Вирівняти</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Горизонтально</translation>
    </message>
    <message>
        <source>Left Sides</source>
        <translation type="obsolete">Ліві сторони</translation>
    </message>
    <message>
        <source>Middles</source>
        <translation type="obsolete">Середини</translation>
    </message>
    <message>
        <source>Right Sides</source>
        <translation type="obsolete">Праві сторони</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Вертикально</translation>
    </message>
    <message>
        <source>Top Sides</source>
        <translation type="obsolete">Верхні сторони</translation>
    </message>
    <message>
        <source>Bottom Sides</source>
        <translation type="obsolete">Нижні сторони</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">піки</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Apply</source>
        <translation type="obsolete">&amp;Застосувати</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Between:</source>
        <translation type="obsolete">&amp;Поміж:</translation>
    </message>
    <message>
        <source>A&amp;lign</source>
        <translation type="obsolete">В&amp;ирівняти</translation>
    </message>
    <message>
        <source>Di&amp;splacement</source>
        <translation type="obsolete">Змі&amp;щення</translation>
    </message>
    <message>
        <source>Distribute &amp;Evenly</source>
        <translation type="obsolete">Розподілити &amp;рівномірно</translation>
    </message>
    <message>
        <source>Bet&amp;ween:</source>
        <translation type="obsolete">&amp;Поміж:</translation>
    </message>
    <message>
        <source>Do &amp;Not Change</source>
        <translation type="obsolete">&amp;Не змінювати</translation>
    </message>
    <message>
        <source>Al&amp;ign</source>
        <translation type="obsolete">В&amp;ирівнювання</translation>
    </message>
    <message>
        <source>Dis&amp;placement</source>
        <translation type="obsolete">Змі&amp;щення</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>Distribute E&amp;venly</source>
        <translation type="obsolete">Розподілити &amp;рівномірно</translation>
    </message>
    <message>
        <source>&amp;Do Not Change</source>
        <translation type="obsolete">&amp;Не змінювати</translation>
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
        <translation type="unfinished">Вирівняти</translation>
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
        <translation type="unfinished">Сторінка</translation>
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
        <translation type="unfinished">Застереження</translation>
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
        <translation>Вирівняти текст по лівому краю</translation>
    </message>
    <message>
        <source>Align Text Right</source>
        <translation>Вирівняти текст по правому краю</translation>
    </message>
    <message>
        <source>Align Text Center</source>
        <translation>Вирівняти текст по центру</translation>
    </message>
    <message>
        <source>Align Text Justified</source>
        <translation>Вирівняти текст з обох сторін</translation>
    </message>
    <message>
        <source>Align Text Forced Justified</source>
        <translation>Змусити вирівняти текст з обох сторін</translation>
    </message>
</context>
<context>
    <name>Annot</name>
    <message>
        <source>Field Properties</source>
        <translation>Властивості поля</translation>
    </message>
    <message>
        <source>Type:</source>
        <translation>Тип:</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation>Властивості</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Ім&apos;я:</translation>
    </message>
    <message>
        <source>Tool-Tip:</source>
        <translation>Підказка по інструменту:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Border</source>
        <translation>Рамка</translation>
    </message>
    <message>
        <source>Color:</source>
        <translation>Колір:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Нічого немає</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation>Товщина:</translation>
    </message>
    <message>
        <source>Thin</source>
        <translation>Тонка</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Звичайна</translation>
    </message>
    <message>
        <source>Wide</source>
        <translation>Широка</translation>
    </message>
    <message>
        <source>Style:</source>
        <translation>Стиль:</translation>
    </message>
    <message>
        <source>Solid</source>
        <translation>Суцільна</translation>
    </message>
    <message>
        <source>Dashed</source>
        <translation>Переривчаста</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation>Підкреслена</translation>
    </message>
    <message>
        <source>Beveled</source>
        <translation>Фасочна</translation>
    </message>
    <message>
        <source>Inset</source>
        <translation>Вкладка</translation>
    </message>
    <message>
        <source>Other</source>
        <translation>Інша</translation>
    </message>
    <message>
        <source>Read Only</source>
        <translation>Лише для читання</translation>
    </message>
    <message>
        <source>Required</source>
        <translation>Необхідне</translation>
    </message>
    <message>
        <source>Don&apos;t Export Value</source>
        <translation>Не експортувати значення</translation>
    </message>
    <message>
        <source>Visibility:</source>
        <translation>Видимість:</translation>
    </message>
    <message>
        <source>Visible</source>
        <translation>Видима</translation>
    </message>
    <message>
        <source>Hidden</source>
        <translation>Схована</translation>
    </message>
    <message>
        <source>No Print</source>
        <translation>Не друкувати</translation>
    </message>
    <message>
        <source>No View</source>
        <translation>Не показувати</translation>
    </message>
    <message>
        <source>Appearance</source>
        <translation>Зовнішній вигляд</translation>
    </message>
    <message>
        <source>Text for Button Down</source>
        <translation>Текста для натиснутої кнопки</translation>
    </message>
    <message>
        <source>Text for Roll Over</source>
        <translation>Текст для зображення, розміщеного поверх</translation>
    </message>
    <message>
        <source>Icons</source>
        <translation>Іконки</translation>
    </message>
    <message>
        <source>Use Icons</source>
        <translation>Використовувати іконки</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Видалити</translation>
    </message>
    <message>
        <source>Pressed</source>
        <translation>Натиснута</translation>
    </message>
    <message>
        <source>Roll Over</source>
        <translation>Розміщення поверх</translation>
    </message>
    <message>
        <source>Icon Placement...</source>
        <translation>Розміщення іконок...</translation>
    </message>
    <message>
        <source>Highlight</source>
        <translation>Виділення</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation>Негатив</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation>Обведена</translation>
    </message>
    <message>
        <source>Push</source>
        <translation>Натиснути</translation>
    </message>
    <message>
        <source>Multi-Line</source>
        <translation>Багаторядкова</translation>
    </message>
    <message>
        <source>Password</source>
        <translation>Пароль</translation>
    </message>
    <message>
        <source>Limit of</source>
        <translation>Обмеження на</translation>
    </message>
    <message>
        <source>Characters</source>
        <translation>Знаки</translation>
    </message>
    <message>
        <source>Do Not Scroll</source>
        <translation>Не прокручувати</translation>
    </message>
    <message>
        <source>Do Not Spell Check</source>
        <translation>Не перевіряти написання</translation>
    </message>
    <message>
        <source>Check Style:</source>
        <translation>Перевірити Стиль:</translation>
    </message>
    <message>
        <source>Default is Checked</source>
        <translation>Вибране значення по умовчанню</translation>
    </message>
    <message>
        <source>Editable</source>
        <translation>Можна редагувати</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Установки</translation>
    </message>
    <message>
        <source>Event:</source>
        <translation>Подія:</translation>
    </message>
    <message>
        <source>Script:</source>
        <translation>Сценарій:</translation>
    </message>
    <message>
        <source>Edit...</source>
        <translation>Редагування...</translation>
    </message>
    <message>
        <source>Submit to URL:</source>
        <translation>Відправити на URL:</translation>
    </message>
    <message>
        <source>Submit Data as HTML</source>
        <translation>Послати дані як HTML</translation>
    </message>
    <message>
        <source>Import Data from:</source>
        <translation>Імпортувати дані з:</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Призначення</translation>
    </message>
    <message>
        <source>To File:</source>
        <translation>Зберегти:</translation>
    </message>
    <message>
        <source>Change...</source>
        <translation>Змінити...</translation>
    </message>
    <message>
        <source>Page:</source>
        <translation>Сторінка:</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Поз. Х:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Поз. У:</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Дія</translation>
    </message>
    <message>
        <source>Field is formatted as:</source>
        <translation>Поле відформатоване як:</translation>
    </message>
    <message>
        <source>Number Format</source>
        <translation>Формат числа</translation>
    </message>
    <message>
        <source>Decimals:</source>
        <translation>Десяткові:</translation>
    </message>
    <message>
        <source>Use Currency Symbol</source>
        <translation>Використовувати знак валюти</translation>
    </message>
    <message>
        <source>Prepend Currency Symbol</source>
        <translation>Розмістити спереду символ валюти</translation>
    </message>
    <message>
        <source>Formatting</source>
        <translation>Формати</translation>
    </message>
    <message>
        <source>Percent Format</source>
        <translation>Формат процентів</translation>
    </message>
    <message>
        <source>Date Format</source>
        <translation>Формат дати</translation>
    </message>
    <message>
        <source>Time Format</source>
        <translation>Формат часу</translation>
    </message>
    <message>
        <source>Custom Scripts</source>
        <translation>Зовнішні сценарії</translation>
    </message>
    <message>
        <source>Format:</source>
        <translation>Формат:</translation>
    </message>
    <message>
        <source>Keystroke:</source>
        <translation>Клавіша:</translation>
    </message>
    <message>
        <source>Format</source>
        <translation>Формат</translation>
    </message>
    <message>
        <source>Value is not validated</source>
        <translation>Значення не перевірене</translation>
    </message>
    <message>
        <source>Value must be greater than or equal to:</source>
        <translation>Значення має бути білье або рівне:</translation>
    </message>
    <message>
        <source>and less or equal to:</source>
        <translation>і менше або рівне:</translation>
    </message>
    <message>
        <source>Custom validate script:</source>
        <translation>Зовнішній сценарій перевірки:</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation>Перевірити</translation>
    </message>
    <message>
        <source>Value is not calculated</source>
        <translation>Значення не вирахуване</translation>
    </message>
    <message>
        <source>Value is the</source>
        <translation>Значення дорівнює</translation>
    </message>
    <message>
        <source>sum</source>
        <translation>Сумма</translation>
    </message>
    <message>
        <source>product</source>
        <translation>помножене</translation>
    </message>
    <message>
        <source>average</source>
        <translation>середнє арифметичне</translation>
    </message>
    <message>
        <source>minimum</source>
        <translation>мінімум</translation>
    </message>
    <message>
        <source>maximum</source>
        <translation>максимум</translation>
    </message>
    <message>
        <source>of the following fields:</source>
        <translation>з наступих полів:</translation>
    </message>
    <message>
        <source>Pick...</source>
        <translation>Вибрати...</translation>
    </message>
    <message>
        <source>Custom calculation script:</source>
        <translation>Зовнішній сценарій обчислення:</translation>
    </message>
    <message>
        <source>Calculate</source>
        <translation>Обчислити</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>Enter a comma separated list of fields here</source>
        <translation>Введіть список полів, розділений комами</translation>
    </message>
    <message>
        <source>You need at least the Icon for Normal to use Icons for Buttons</source>
        <translation>Вам потрібна, як мінімум, іконка для Нормального, щоб використовувати іконки для кнопок</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;Postscript (*.eps);;All Files (*)</source>
        <translation type="obsolete">Зображення (*.tif *.png *.jpg *.xpm);;Постскрипт (*.eps);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Example:</source>
        <translation>Приклад:</translation>
    </message>
    <message>
        <source>Selection Change</source>
        <translation>Зміни вибірки</translation>
    </message>
    <message>
        <source>Java Script</source>
        <translation type="obsolete">Сценарій на мові Java</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Кнопка</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Текстове поле</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Вибіркове поле</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Комбінований випадаючий список</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Випадаючий список</translation>
    </message>
    <message>
        <source>Check</source>
        <translation>Галочка</translation>
    </message>
    <message>
        <source>Cross</source>
        <translation>Хрест</translation>
    </message>
    <message>
        <source>Diamond</source>
        <translation>Ромб</translation>
    </message>
    <message>
        <source>Circle</source>
        <translation>Коло</translation>
    </message>
    <message>
        <source>Star</source>
        <translation>Зірка</translation>
    </message>
    <message>
        <source>Square</source>
        <translation>Квадрат</translation>
    </message>
    <message>
        <source>Go To</source>
        <translation>Перейти до</translation>
    </message>
    <message>
        <source>Submit Form</source>
        <translation>Відправити форму</translation>
    </message>
    <message>
        <source>Reset Form</source>
        <translation>Очистити форму</translation>
    </message>
    <message>
        <source>Import Data</source>
        <translation>Імпортувати дані</translation>
    </message>
    <message>
        <source>Mouse Up</source>
        <translation>Миша вгору</translation>
    </message>
    <message>
        <source>Mouse Down</source>
        <translation>Миша вниз</translation>
    </message>
    <message>
        <source>Mouse Enter</source>
        <translation>Миша вхід</translation>
    </message>
    <message>
        <source>Mouse Exit</source>
        <translation>Миша вихід</translation>
    </message>
    <message>
        <source>On Focus</source>
        <translation>На фокусування</translation>
    </message>
    <message>
        <source>On Blur</source>
        <translation>На розмивання</translation>
    </message>
    <message>
        <source>Plain</source>
        <translation>Звичайний</translation>
    </message>
    <message>
        <source>Number</source>
        <translation>Номер</translation>
    </message>
    <message>
        <source>Percentage</source>
        <translation>Частка</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
    <message>
        <source>Time</source>
        <translation>Час</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Нестандартний</translation>
    </message>
    <message>
        <source>Font for use with PDF 1.3:</source>
        <translation>Шрифт для використання в PDF 1.3:</translation>
    </message>
    <message>
        <source>Flag is ignored for PDF 1.3</source>
        <translation>Установка ігнорується в PDF 1.3</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF файли (*.pdf);;Всі файли (*)</translation>
    </message>
    <message>
        <source>JavaScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Images (*.tif *.png *.jpg *.xpm);;PostScript (*.eps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Annota</name>
    <message>
        <source>Annotation Properties</source>
        <translation>Властивості аннотації</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ссилка</translation>
    </message>
    <message>
        <source>External Link</source>
        <translation>Зовнішня ссилка</translation>
    </message>
    <message>
        <source>External Web-Link</source>
        <translation>Зовнішня ссилка на світову мережу</translation>
    </message>
    <message>
        <source>Destination</source>
        <translation>Призначення</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>точок</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>PDF-Documents (*.pdf);;All Files (*)</source>
        <translation>Документи PDF (*.pdf);;Всі файли (*)</translation>
    </message>
    <message>
        <source>&amp;Type:</source>
        <translation>&amp;Тип:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>&amp;Page:</source>
        <translation>&amp;Сторінка:</translation>
    </message>
    <message>
        <source>&amp;X-Pos</source>
        <translation>Поз. &amp;Х</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Поз. &amp;У:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>ApplyMasterPageDialog</name>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation type="unfinished">&amp;Всі сторінки</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
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
        <source>&lt;qt&gt;Apply the selected template to even, odd or all pages within the following range&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>to</source>
        <translation type="unfinished">до</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
</context>
<context>
    <name>ApplyT</name>
    <message>
        <source>Apply Template</source>
        <translation type="obsolete">Використати шаблон</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Звичайний</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Шаблон:</translation>
    </message>
    <message>
        <source>Apply to &amp;Current Page</source>
        <translation type="obsolete">Застосувати  на &amp;активній сторінці</translation>
    </message>
    <message>
        <source>Apply from &amp;Page:</source>
        <translation type="obsolete">Застосувати  &amp;з сторінки:</translation>
    </message>
    <message>
        <source>To:</source>
        <translation type="obsolete">До:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Apply to all &amp;even Pages</source>
        <translation type="obsolete">Застосувати на всіх &amp;парних сторінках</translation>
    </message>
    <message>
        <source>Apply to all &amp;odd Pages</source>
        <translation type="obsolete">Застосувати на всіх &amp;непарних сторінках</translation>
    </message>
</context>
<context>
    <name>ArrowChooser</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Biblio</name>
    <message>
        <source>Scrapbook</source>
        <translation>Чорновик</translation>
    </message>
    <message>
        <source>Scrapbooks (*.scs);;All Files (*)</source>
        <translation>Чорновики (*.scs);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation>Видалити</translation>
    </message>
    <message>
        <source>Object</source>
        <translation>Об&apos;єкт</translation>
    </message>
    <message>
        <source>New Entry</source>
        <translation>Новий екземпляр</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation>Перейменувати</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ім&apos;я &quot;%1&quot; вже використане.
Будь-ласка виберіть інше.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Load...</source>
        <translation>За&amp;вантажити...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Записати</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Записати &amp;як...</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Зачинити</translation>
    </message>
    <message>
        <source>&amp;Small</source>
        <translation>&amp;Малий</translation>
    </message>
    <message>
        <source>&amp;Medium</source>
        <translation>&amp;Середній</translation>
    </message>
    <message>
        <source>&amp;Large</source>
        <translation>&amp;Великий</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Preview</source>
        <translation>&amp;Попередній перегляд</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ім&apos;я:</translation>
    </message>
</context>
<context>
    <name>BookMView</name>
    <message>
        <source>Bookmarks</source>
        <translation>Закладки</translation>
    </message>
    <message>
        <source>Move Bookmark</source>
        <translation>Перемістити закладку</translation>
    </message>
    <message>
        <source>Insert Bookmark</source>
        <translation>Вставити закладку</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
</context>
<context>
    <name>BookPalette</name>
    <message>
        <source>Bookmarks</source>
        <translation>Закладки</translation>
    </message>
</context>
<context>
    <name>ButtonIcon</name>
    <message>
        <source>Icon Placement</source>
        <translation>Розміщення іконок</translation>
    </message>
    <message>
        <source>Layout:</source>
        <translation>Схема:</translation>
    </message>
    <message>
        <source>Scale:</source>
        <translation>Масштаб:</translation>
    </message>
    <message>
        <source>Always</source>
        <translation>Завжди</translation>
    </message>
    <message>
        <source>When Icon is too small</source>
        <translation>Коли іконка занадто мала</translation>
    </message>
    <message>
        <source>When Icon is too big</source>
        <translation>Коли іконка занадто велика</translation>
    </message>
    <message>
        <source>Never</source>
        <translation>Ніколи</translation>
    </message>
    <message>
        <source>Scale How:</source>
        <translation>Змінити масштаб на:</translation>
    </message>
    <message>
        <source>Proportional</source>
        <translation>Пропорційно</translation>
    </message>
    <message>
        <source>Non Proportional</source>
        <translation>Непропорційно</translation>
    </message>
    <message>
        <source>Icon</source>
        <translation>Іконка</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>Reset</source>
        <translation>Скинути значення</translation>
    </message>
    <message>
        <source>Caption only</source>
        <translation>Лише описання</translation>
    </message>
    <message>
        <source>Icon only</source>
        <translation>Лише іконка</translation>
    </message>
    <message>
        <source>Caption below Icon</source>
        <translation>Описання під іконкою</translation>
    </message>
    <message>
        <source>Caption above Icon</source>
        <translation>Описання над іконкою</translation>
    </message>
    <message>
        <source>Caption right to Icon</source>
        <translation>Описання справа від іконки</translation>
    </message>
    <message>
        <source>Caption left to Icon</source>
        <translation>Описання зліва від іконки</translation>
    </message>
    <message>
        <source>Caption overlays Icon</source>
        <translation>Описання поверх іконки</translation>
    </message>
</context>
<context>
    <name>CMSPrefs</name>
    <message>
        <source>Color Management Settings</source>
        <translation type="obsolete">Установка палітри кольорів</translation>
    </message>
    <message>
        <source>System Profiles</source>
        <translation>Системні профілі</translation>
    </message>
    <message>
        <source>Rendering Intents</source>
        <translation>Схеми перерахунку кольорів</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Уявний</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Відносна кольорометрія</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насичення</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Абсолютна кольорометрія</translation>
    </message>
    <message>
        <source>Default color profile for imported images</source>
        <translation type="obsolete">Стандартний кольоровий профіль для імпортованих зображень</translation>
    </message>
    <message>
        <source>Default color profile for solid colors on the page</source>
        <translation>Стандартний кольоровий профіль для однотонних кольорів на сторінці</translation>
    </message>
    <message>
        <source>Color profile that you have generated or received from the manufacturer.
This profile should be specific to your monitor and not a generic profile (i.e. sRGB).</source>
        <translation>Кольоровий профіль побудований Вами або отриманий від виробника обладнання.
Цей профіль має точно відповідати Вашому монітору, а не бути загальним профілем (типу sRGB).</translation>
    </message>
    <message>
        <source>Color profile for your printer model from the manufacturer.
This profile should be specific to your printer and not a generic profile (i.e. sRGB).</source>
        <translation>Кольоровий профіль для Вашої моделі прінтера отриманий від виробника обладнання.
Цей профіль має точно відповідати Вашому монітору, а не бути загальним профілем (типу sRGB).</translation>
    </message>
    <message>
        <source>Default rendering intent for your monitor. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Стандартна схема перерахунку кольорів для Вашого монітора. Не змінюйте
її без необхідності. У більшості випадків краще вибрати відносну колориметричну
або перцептивну схему.</translation>
    </message>
    <message>
        <source>Default rendering intent for your printer. Unless you know why to change it,
Relative Colorimetric or Perceptual should be chosen.</source>
        <translation>Стандартна схема перерахунку кольорів для Вашого прінтера. Не змінюйте
її без необхідності. У більшості випадків краще вибрати відносну колориметричну
або перцептивну схему.</translation>
    </message>
    <message>
        <source>Enable &apos;soft proofing&apos; of how your document colors will print,
based on the chosen printer profile.</source>
        <translation>Ввімкнути імітацію друкованих кольорів на моніторі (м&apos;яка перевірка),
в залежності від вибраного профілю прінтера.</translation>
    </message>
    <message>
        <source>Method of showing colors on the screen which may not print properly.
This requires very accurate profiles and serves only as a warning.</source>
        <translation>Метод відображення на екрані тих кольорів, які не будуть правильно надруковані.
Вимагає наявності дуже точних кольорових профілів і служить лише як застереження.</translation>
    </message>
    <message>
        <source>Black Point Compensation is a method of improving contrast in photos.
It is recommended that you enable this if you have photos in your document.</source>
        <translation>Компенсація точки чорного - це метод покращення контрастності фотографічних зображень.
Її використаннярекомендується при наявності фотографічних зображень в документі.</translation>
    </message>
    <message>
        <source>&amp;Activate Color Management</source>
        <translation>Активувати &amp;керування палітрою кольорів</translation>
    </message>
    <message>
        <source>&amp;Pictures:</source>
        <translation type="obsolete">Зо&amp;браження:</translation>
    </message>
    <message>
        <source>&amp;Solid Colors:</source>
        <translation>&amp;Однотонні кольори:</translation>
    </message>
    <message>
        <source>&amp;Monitor:</source>
        <translation>&amp;Екран:</translation>
    </message>
    <message>
        <source>P&amp;rinter:</source>
        <translation>Прин&amp;тер:</translation>
    </message>
    <message>
        <source>M&amp;onitor:</source>
        <translation>Екр&amp;ан:</translation>
    </message>
    <message>
        <source>Pr&amp;inter:</source>
        <translation>Пр&amp;интер:</translation>
    </message>
    <message>
        <source>Sim&amp;ulate Printer on the Screen</source>
        <translation>Сим&amp;улювати принтер на екрані</translation>
    </message>
    <message>
        <source>Mark Colors out of &amp;Gamut</source>
        <translation>Позначити кольори, які знаходяться &amp;поза гамутом</translation>
    </message>
    <message>
        <source>Use &amp;Blackpoint Compensation</source>
        <translation>Використовувати компенсаці&amp;ю точки чорного</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
        <translation>Редагування кольору</translation>
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
        <translation>RGB для web</translation>
    </message>
    <message>
        <source>New</source>
        <translation>Новий</translation>
    </message>
    <message>
        <source>Old</source>
        <translation>Старий</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
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
        <translation>Динамічні кольорові полоси</translation>
    </message>
    <message>
        <source>Static Color Bars</source>
        <translation>Статичні кольорові полоси</translation>
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
        <translation>%</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Name of the Color is not unique</source>
        <translation>Ім&apos;я кольору уже використане</translation>
    </message>
    <message>
        <source>HSV-Colormap</source>
        <translation>HSV (ВНЗ) Палітра кольорів</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ім&apos;я:</translation>
    </message>
    <message>
        <source>Color &amp;Model</source>
        <translation>Кольорова &amp;модель</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Нічого</translation>
    </message>
    <message>
        <source>You cannot create a color named &quot;%1&quot;.
It&apos;s a reserved name for transparent color</source>
        <translation>Ви не можете створити колір під назвою &quot;%1&quot;.
Це ім&apos;я зарезервоване для прозорого кольору</translation>
    </message>
    <message>
        <source>Is Spot Color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Is Registration Color</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ChTable</name>
    <message>
        <source>You can see a thumbnail if you press
and hold down the right mouse button

The Insert key inserts a Glyph into the Selection below
and the Delete key removes the last inserted one</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CharSelect</name>
    <message>
        <source>Select Character:</source>
        <translation type="unfinished">Вибрати символ:</translation>
    </message>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Шрифт:</translation>
    </message>
    <message>
        <source>Character Class:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Вставити</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">О&amp;чистити</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Зачинити</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="unfinished">Вставити символи в текст в місці знаходження курсора</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="unfinished">Видалити активну вибірку.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="unfinished">Зачинити цей діалог та повернутися до редагування тексту.</translation>
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
        <translation type="unfinished">Грецька</translation>
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
        <translation type="unfinished">Документ</translation>
    </message>
    <message>
        <source>No Problems found</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Сторінка</translation>
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
        <translation type="obsolete">Постскрипт</translation>
    </message>
</context>
<context>
    <name>ChooseStyles</name>
    <message>
        <source>Choose Styles</source>
        <translation>Вибрати стилі</translation>
    </message>
    <message>
        <source>Available Styles</source>
        <translation>Доступні стилі</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
    </message>
</context>
<context>
    <name>CollectForOutput</name>
    <message>
        <source>Choose a Directory</source>
        <translation type="unfinished">Вибрати директорію</translation>
    </message>
    <message>
        <source>Collecting...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Застереження</translation>
    </message>
    <message>
        <source>Cannot collect all files for output for file:
%1</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Колір</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Ім&apos;я</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="unfinished">С</translation>
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
        <translation type="obsolete">В&amp;ихід</translation>
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
        <source>Protanopia (red)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deuteranopia (green)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Tritanopia (blue)</source>
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
        <translation type="unfinished">&amp;Колір</translation>
    </message>
    <message>
        <source>Saturation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Difference between the selected value and the counted ones. Refer to documentation for more information.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Saturation component in HSV mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Value component in HSV mode</source>
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
        <translation type="unfinished">Новий колір</translation>
    </message>
    <message>
        <source>Unable to find the requested color. You have probably selected black, gray or white. There is no way to process this color.</source>
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
        <translation type="unfinished">&amp;Застосувати</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">В&amp;ихід</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Записати</translation>
    </message>
</context>
<context>
    <name>Cpalette</name>
    <message>
        <source>Normal</source>
        <translation>Нормальне</translation>
    </message>
    <message>
        <source>Horizontal Gradient</source>
        <translation>Горизонтальний градієнт</translation>
    </message>
    <message>
        <source>Vertical Gradient</source>
        <translation>Вертикальний градієнт</translation>
    </message>
    <message>
        <source>Diagonal Gradient</source>
        <translation>Діагональний градієнт</translation>
    </message>
    <message>
        <source>Cross Diagonal Gradient</source>
        <translation>Крос-діагональний градієнт</translation>
    </message>
    <message>
        <source>Radial Gradient</source>
        <translation>Радіальний градієнт</translation>
    </message>
    <message>
        <source>Opacity:</source>
        <translation>Непрозорість:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation>Тінь:</translation>
    </message>
    <message>
        <source>Edit Line Color Properties</source>
        <translation>Редагувати установки кольору лінії</translation>
    </message>
    <message>
        <source>Edit Fill Color Properties</source>
        <translation>Редагувати установки кольору заливки</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation>Насиченість кольору</translation>
    </message>
    <message>
        <source>Normal or gradient fill method</source>
        <translation>Нормальний або градієнтний метод заливки</translation>
    </message>
    <message>
        <source>Set the transparency for the color selected</source>
        <translation>Зробити вибраний колір прозорим</translation>
    </message>
    <message>
        <source>Color of selected object</source>
        <translation type="obsolete">Колір вибраного об&apos;єкта</translation>
    </message>
    <message>
        <source>Free linear Gradient</source>
        <translation>Вільний лінійний градієнт</translation>
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
        <translation>тчк</translation>
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
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>Free radial Gradient</source>
        <translation>Вільний радіальний градієнт</translation>
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
        <translation>Установки імпорту csv</translation>
    </message>
    <message>
        <source>Field delimiter:</source>
        <translation>Розділювач полів:</translation>
    </message>
    <message>
        <source>(TAB)</source>
        <translation>(Таб)</translation>
    </message>
    <message>
        <source>Value delimiter:</source>
        <translation>Розділювач значень:</translation>
    </message>
    <message>
        <source>First row is a header</source>
        <translation>Перший ряд містить заголовки</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
</context>
<context>
    <name>CupsOptions</name>
    <message>
        <source>Printer Options</source>
        <translation>Установки принтера</translation>
    </message>
    <message>
        <source>Option</source>
        <translation>Опція</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Значення</translation>
    </message>
    <message>
        <source>Page Set</source>
        <translation>Набір сторінок</translation>
    </message>
    <message>
        <source>All Pages</source>
        <translation>Всі сторінки</translation>
    </message>
    <message>
        <source>Even Pages only</source>
        <translation>Лише парні сторінки</translation>
    </message>
    <message>
        <source>Odd Pages only</source>
        <translation>Лише непарні сторінки</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Дзеркальне зображення</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ні</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Так</translation>
    </message>
    <message>
        <source>Orientation</source>
        <translation>Орієнтація</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Горизонтальна</translation>
    </message>
    <message>
        <source>N-Up Printing</source>
        <translation>Х сторінок лицем догори</translation>
    </message>
    <message>
        <source>Page per Sheet</source>
        <translation>Сторінка на лист паперу</translation>
    </message>
    <message>
        <source>Pages per Sheet</source>
        <translation>Сторінок на лист паперу</translation>
    </message>
    <message>
        <source>This panel displays various CUPS options when printing. 
The exact parameters available will depend on your printer driver.
You can confirm CUPS support by selecting Help &gt; About.
Look for the listings: C-C-T These equate to C=CUPS C=littlecms T=TIFF support.
Missing library support is indicated by a *</source>
        <translation>Ця панель показує параметри CUPS при друці.
Набір доступних параметрів залежить від 
драйвера прінтера. Наявність підтримки CUPS
можна засвідчити вибравши меню 
&quot;Допомога&gt;Про програму Scribus&quot;. Зверніть увагу на
літери індикатори С-С-T, які можна розібрати як 
С=CUPS - Загальна система друку для Юнікс, 
С=littlecms - Проста система керування кольором, 
T=TIFF - Підтримка формату зображень TIFF. 
При відсутності бібліотеки літера індикатор 
заміщається на *</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>CustomFDialog</name>
    <message>
        <source>Encoding:</source>
        <translation>Кодування:</translation>
    </message>
    <message>
        <source>Moves to your Document Directory.
This can be set in the Preferences.</source>
        <translation>Переміщує Вас в вашу домашню директорію.
Її можна установити заздалегідь в Опціях.</translation>
    </message>
    <message>
        <source>&amp;Compress File</source>
        <translation>&amp;Стиснути файл</translation>
    </message>
    <message>
        <source>&amp;Include Fonts</source>
        <translation>Включити &amp;шрифти</translation>
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
        <translation>Видалити колір</translation>
    </message>
    <message>
        <source>?</source>
        <translation type="obsolete">?</translation>
    </message>
    <message>
        <source>Replace it with:</source>
        <translation type="obsolete">Замінити це на:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
    </message>
    <message>
        <source>Delete color:</source>
        <translation type="obsolete">Видалити колір:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Нічого</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
        <translation>Видалення сторінок</translation>
    </message>
    <message>
        <source>Delete from:</source>
        <translation type="obsolete">Видалити з:</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>до:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Delete From:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DmF</name>
    <message>
        <source>Missing Font</source>
        <translation type="obsolete">Відсутній шрифт</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="obsolete">Шрифт %1 не встановлений.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="obsolete">Використати</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="obsolete">замість</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
</context>
<context>
    <name>DocInfos</name>
    <message>
        <source>Document Information</source>
        <translation>Інформація про документ</translation>
    </message>
    <message>
        <source>&amp;Title:</source>
        <translation>&amp;Заголовок:</translation>
    </message>
    <message>
        <source>&amp;Author:</source>
        <translation>&amp;Автор:</translation>
    </message>
    <message>
        <source>&amp;Keywords:</source>
        <translation>&amp;Ключові слова:</translation>
    </message>
    <message>
        <source>Descri&amp;ption:</source>
        <translation>&amp;Опис:</translation>
    </message>
    <message>
        <source>P&amp;ublisher:</source>
        <translation>&amp;Видавник:</translation>
    </message>
    <message>
        <source>&amp;Contributors:</source>
        <translation>&amp;Співробітники:</translation>
    </message>
    <message>
        <source>Dat&amp;e:</source>
        <translation>&amp;Дата:</translation>
    </message>
    <message>
        <source>T&amp;ype:</source>
        <translation>&amp;Тип:</translation>
    </message>
    <message>
        <source>F&amp;ormat:</source>
        <translation>&amp;Формат:</translation>
    </message>
    <message>
        <source>Identi&amp;fier:</source>
        <translation>В&amp;изначник:</translation>
    </message>
    <message>
        <source>&amp;Source:</source>
        <translation>Д&amp;жерело:</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Мова:</translation>
    </message>
    <message>
        <source>&amp;Relation:</source>
        <translation>В&amp;ідношення:</translation>
    </message>
    <message>
        <source>Co&amp;verage:</source>
        <translation>&amp;Покриття:</translation>
    </message>
    <message>
        <source>Ri&amp;ghts:</source>
        <translation>П&amp;рава:</translation>
    </message>
    <message>
        <source>&amp;Document</source>
        <translation type="obsolete">Док&amp;умент</translation>
    </message>
    <message>
        <source>Further &amp;Information</source>
        <translation>Д&amp;одаткова інформація</translation>
    </message>
    <message>
        <source>The person or organisation primarily responsible for making the content of the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Людина чи організація, відповідальна за створення документа,
Це поле може бути вміщене в документ Scribus для довідки а також в
метадані PDF</translation>
    </message>
    <message>
        <source>A name given to the document.
This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF</source>
        <translation type="obsolete">Ім&apos;я дане документу.
Це поле може бути вміщене в документ Scribus, а також в метадані PDF</translation>
    </message>
    <message>
        <source>An account of the content of the document.
This field is for a brief description or abstract of the document. It is embedded in the PDF on export</source>
        <translation type="obsolete">Огляд змісту документа. Це поле призначається для короткого пояснення або абстракту документа.
Воно вміщується в PDF при експорті</translation>
    </message>
    <message>
        <source>The topic of the content of the document.
This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files</source>
        <translation type="obsolete">Це заголовок змісту документа. Це поле призначається для ключових слів документа, які ви бажаєте 
вмістити в PDF для полегшення пошуків та індексування файлів PDF</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making the document available</source>
        <translation>Людина чи організаця відповідальна за випуск документа в загальний доступ</translation>
    </message>
    <message>
        <source>A person or organisation responsible for making contributions to the content of the document</source>
        <translation>Людина чи організація відповідальна за вклад в зміст документа</translation>
    </message>
    <message>
        <source>A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601</source>
        <translation>Дата зв&apos;язана з подією в життєвому  циклі документа в форматі РРРР-ММ-ДД, як вказано в ISO 8601</translation>
    </message>
    <message>
        <source>The nature or genre of the content of the document, eg. categories, functions, genres, etc</source>
        <translation>Жанр змісту документа - наприклад категорії, функції, жанри і так далі</translation>
    </message>
    <message>
        <source>The physical or digital manifestation of the document. Media type and dimensions would be worth noting.
RFC2045,RFC2046 for MIME types are also useful here</source>
        <translation type="obsolete">Фізичне чи цифрове відображення документа. Тип паперу та розміри можуть бути цікавими.
Тут можить придатися RFC2045 і RFC2046 для типів MIME</translation>
    </message>
    <message>
        <source>An unambiguous reference to the document within a given context such as ISBN or URI</source>
        <translation>Безумовна ссилка на документ у даному контексті, такому як ISBN чи URI</translation>
    </message>
    <message>
        <source>The language in which the content of the document is written, usually a ISO-639 language code
optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH</source>
        <translation type="obsolete">Мова, на якій написано документ, зазвичай мовний код з ISO-639 інколи з додатковим суфіксом через
тире і код країни з ISO-3166. Наприклад en-GB, fr-CH</translation>
    </message>
    <message>
        <source>A reference to a related document, possibly using a formal identifier such as a ISBN or URI</source>
        <translation>Ссилка на споріднений документ, можливо використовуючи ідентифікатор формату, такий як ISBN чи URI</translation>
    </message>
    <message>
        <source>The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges</source>
        <translation>Масштаб чи протяжність документа, можливо включаючи розміщення, час та діапазони юрисдикції</translation>
    </message>
    <message>
        <source>Information about rights held in and over the document, eg. copyright, patent or trademark</source>
        <translation>Інформація про права на документ. Наприклад копірайт, патент, чи торгова марка</translation>
    </message>
    <message>
        <source>A reference to a document from which the present document is derived, eg. ISBN or URI</source>
        <translation>Ссилка на документ який послужив основою для данного документа, напр. ISBN чи URI</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
    <name>DocumentItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <source>Document Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Ім&apos;я</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Тип</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Значення</translation>
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
        <translation type="unfinished">&amp;Додати</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Скопіювати</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">О&amp;чистити</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
</context>
<context>
    <name>Druck</name>
    <message>
        <source>Setup Printer</source>
        <translation>Налаштування принтера</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Установки</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Всі кольори</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>Postscript-Files (*.ps);;All Files (*)</source>
        <translation type="obsolete">Постскрипт файли (*.ps);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation>Салатовий</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation>Малиновий</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation>Жовтий</translation>
    </message>
    <message>
        <source>Black</source>
        <translation>Чорний</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Вмістити список елементів, розділений комами де елемент
може &quot;бути для всіх сторінок, 1-5 для діапазона сторінок, або
номер окремої сторінки.</translation>
    </message>
    <message>
        <source>Print Destination</source>
        <translation>Призначення для друку</translation>
    </message>
    <message>
        <source>&amp;Options...</source>
        <translation>&amp;Установки...</translation>
    </message>
    <message>
        <source>&amp;File:</source>
        <translation>&amp;Файл:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>A&amp;lternative Printer Command</source>
        <translation>&amp;Альтернативна команда для принтера</translation>
    </message>
    <message>
        <source>Co&amp;mmand:</source>
        <translation>&amp;Команда:</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Діапазон</translation>
    </message>
    <message>
        <source>Print &amp;All</source>
        <translation>Друкувати &amp;все</translation>
    </message>
    <message>
        <source>Print Current Pa&amp;ge</source>
        <translation>Друкувати активну &amp;сторінку</translation>
    </message>
    <message>
        <source>Print &amp;Range</source>
        <translation>Друкувати &amp;діапазон</translation>
    </message>
    <message>
        <source>N&amp;umber of Copies:</source>
        <translation>Число ко&amp;пій:</translation>
    </message>
    <message>
        <source>Print &amp;Normal</source>
        <translation type="obsolete">&amp;Звичайний друк</translation>
    </message>
    <message>
        <source>Print &amp;Separations</source>
        <translation type="obsolete">Друкувати &amp;сепарації кольорів</translation>
    </message>
    <message>
        <source>Pr&amp;int In Color If Available</source>
        <translation type="obsolete">Друкувати в &amp;кольорі, якщо можливо</translation>
    </message>
    <message>
        <source>Print In Gra&amp;yscale</source>
        <translation type="obsolete">Друкувати у відтінках &amp;сірого</translation>
    </message>
    <message>
        <source>Ad&amp;vanced Options...</source>
        <translation type="obsolete">&amp;Додаткові установки...</translation>
    </message>
    <message>
        <source>&amp;Print</source>
        <translation>&amp;Друк</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Use an alternative print manager, such as kprinter or gtklp,
to utilize additional printing options</source>
        <translation>Використати альтернативний менеджер друку, наприклад krpinter
чи gtklp, для доступу до додаткових установок друку</translation>
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
        <translation type="unfinished">Сторінка</translation>
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
        <translation type="unfinished">Установити розмір паперу</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Колір</translation>
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
        <translation type="unfinished">Додаткові установки</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript Files (*.ps);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>EPSPlug</name>
    <message>
        <source>Importing File:
%1
failed!</source>
        <translation>Імпортувати файл:
%1
не вдалося!</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Фатальна помилка</translation>
    </message>
</context>
<context>
    <name>EditMacroDialog</name>
    <message>
        <source>Editing Macro: &lt;b&gt;</source>
        <translation type="obsolete">Редагування макроса: &lt;b&gt;</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Менеджер макросів</translation>
    </message>
    <message>
        <source>The file &apos;%1&apos; already exists.
Are you sure you want to overwrite it?
</source>
        <translation type="obsolete">Файл &apos;%1&apos; вже існує.
Ви впевнені в тому, що Ви бажаєте його переписати?
</translation>
    </message>
    <message>
        <source>You have already edited this macro.
Are you sure you want to discard all your changes?
</source>
        <translation type="obsolete">Ви вже редагували цей макрос. Ви впевнені, що
бажаєте відмінити всі внесені зміни?
</translation>
    </message>
    <message>
        <source>A full traceback follows:

%1
</source>
        <translation type="obsolete">Повне описання помилки:

%1
</translation>
    </message>
    <message>
        <source>Compilation of the macro failed, so it can not 
be saved in its current form. The error was:
%1
</source>
        <translation type="obsolete">Компіляція макроса не була успішною, тому він
не може бути записаним в теперішній формі.
Помилка:
%1
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Новий макрос</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This is the Edit Macro / New Macro dialog box. Here you can change the source code to macros. Edit the source code to the macro in the text editing area below the &quot;Source Code&quot; label and click OK to save your changes to the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Це діалогове вікно Редагувати Макрос / Новий Макрос. Тут Ви можете змінити вихідний код макросів. Відредагуйте вихідний код макроса в зоні редагування тексту під етикеткою &quot;Вихідний код&quot; та натисніть на кнопку Гаразд, щоб зберегти зроблені зміни в макросі.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Source Code:</source>
        <translation type="obsolete">Вихідний код:</translation>
    </message>
    <message>
        <source>Editing Macro:</source>
        <translation type="obsolete">Редагування Макроса:</translation>
    </message>
    <message>
        <source>The name of the macro being edited.</source>
        <translation type="obsolete">Ім&apos;я редагованого макроса.</translation>
    </message>
    <message>
        <source>&lt;/qt&gt;This item displays the name of the macro you are currently editing.&lt;qt&gt;</source>
        <translation type="obsolete">&lt;/qt&gt;Цей об&apos;єкт вказує ім&apos;я редагованого макроса.&lt;qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Discard all changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Відмінити всі зміни та вийти.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Exit the editing dialog, discarding all changes you have made. If you want to exit without saving the macro but don&apos;t want to lose your changes, save your changes with &quot;Save Source As...&quot;.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Вийти з діалога редагування відміняючи зроблені зміни. Якщо Ви бажаєте вийте без збереження цього макроса, але не хочете втратити зроблені зміни - збережіть Ваші зміни з допомогою &quot;Зберегти вихідний код як...&quot;.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes and exit.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Записати зміни та вийти.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save changes to the macro and exit. If there is a problem with the macro, a message will be displayed and the editing dialog will not close.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Записати зміни в макросі та вийти. Якщо в макросі є проблема Вам буде показано повідомлення і діалогове вікно редагування не буде зачинено.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;This text area contains the source code of the macro. If you&apos;re creating a new macro there won&apos;t be anything in it, and if you&apos;re editing an existing macro the source code the macro was defined with will be shown here.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Ця текстова область містить вихідний код макроса. Якщо Ви створюєте новий макрос в ній ще нічого не буде, а якщо Ви редагуєте існуючий макрос, його вихідний код буде в ній показаним.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Load Source ...</source>
        <translation type="obsolete">&amp;Завантажити вихідний код...</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="obsolete">Alt+L</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace the current source code with code from a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Замістити теперішній вихідний код кодом з файла.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Load new source code into the editing area from &quot;file&quot;. Any source code in the editing area is replaced. The loaded source must be a Scribus macro function. If you load any other script, you&apos;ll need to edit it so that it&apos;ll work as a scripter macro before saving it.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Завантажити новий вихідний код в область редагування з файла &quot;file&quot;. Весь вихідний код в області редагування буде заміщено. Завантажений код повинен бути макро функцією Scribus. Якщо Ви завантажите любий інший тип скрипта, Вам прийдеться його відредагувати так, щоб він працював як макрос сценариста перед тим як Ви спробуєте його записати.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Save Source As...</source>
        <translation type="obsolete">&amp;Записати вихідний код як...</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Save the source code being edited to a file.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Записати редагований вихідний код у файл.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Save the source code - the text - of the macro to a file. You can edit the saved source and load it again with &quot;Load Source...&quot;.</source>
        <translation type="obsolete">Записати вихідний код макроса у файл. Ви зможете редагувати записаний вихідний код і завантажити його знову через &quot;Завантажити вихідний код...&quot;.</translation>
    </message>
    <message>
        <source>Python source files (.py)</source>
        <translation type="obsolete">Файли з вихідним кодом на мові Пітон (.py)</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Діалогове вікно Записати Файл</translation>
    </message>
    <message>
        <source>Save macro source</source>
        <translation type="obsolete">Записати вихідний код макроса</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Діалогове вікно Відчинити Файл</translation>
    </message>
    <message>
        <source>Select the source file to load</source>
        <translation type="obsolete">Вибрати файл з вихідним кодом для завантаження</translation>
    </message>
</context>
<context>
    <name>EditStyle</name>
    <message>
        <source>Edit Style</source>
        <translation>Редагування стилю</translation>
    </message>
    <message>
        <source>Character</source>
        <translation>Символ</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>Effect:</source>
        <translation type="obsolete">Ефект:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
    <message>
        <source>Vertical Spaces</source>
        <translation type="obsolete">Вертикальні проміжки</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Інтервал між рядками</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Name of the Style is not unique</source>
        <translation>Ім&apos;я стилю вже використане</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">пік</translation>
    </message>
    <message>
        <source>Name of your paragraph style</source>
        <translation>Назвіть Ваш стиль абзаца</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Шрифт вибраного тексту чи об&apos;єкту</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Розмір шрифта</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Колір заповнення знаків</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Колір знакових силуетів</translation>
    </message>
    <message>
        <source>Provides an oversized first letter for a paragraph. Used for stylistic effect</source>
        <translation type="obsolete">Буквиця - дуже збільшена перша літера абзаца. Використовується, як стилістичний ефект</translation>
    </message>
    <message>
        <source>Determines the overall height, in line numbers, of the Drop Caps</source>
        <translation>Визначає загальну висоту буквиці числом рядків тексту</translation>
    </message>
    <message>
        <source>Align text to baseline grid</source>
        <translation type="obsolete">Вирівняти текст по базовій сітці</translation>
    </message>
    <message>
        <source>Spacing above the paragraph</source>
        <translation>Проміжок над абзацом</translation>
    </message>
    <message>
        <source>Spacing below the paragraph</source>
        <translation>Проміжок під абзацом</translation>
    </message>
    <message>
        <source>Tabulators and Indentation</source>
        <translation>Табулятори та відступи</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ім&apos;я:</translation>
    </message>
    <message>
        <source>&amp;Font:</source>
        <translation type="obsolete">&amp;Шрифт:</translation>
    </message>
    <message>
        <source>Si&amp;ze:</source>
        <translation type="obsolete">&amp;Розмір:</translation>
    </message>
    <message>
        <source>&amp;Alignment:</source>
        <translation type="obsolete">Ви&amp;рівнювання:</translation>
    </message>
    <message>
        <source>&amp;Drop Caps</source>
        <translation type="obsolete">&amp;Буквиця</translation>
    </message>
    <message>
        <source>&amp;Lines:</source>
        <translation>&amp;Лінії:</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Колір запо&amp;внення:</translation>
    </message>
    <message>
        <source>St&amp;roke Color:</source>
        <translation type="obsolete">Колір кон&amp;тура:</translation>
    </message>
    <message>
        <source>Adjust to Baseline &amp;Grid</source>
        <translation type="obsolete">Вирівняти текст по &amp;базовій сітці</translation>
    </message>
    <message>
        <source>Line &amp;Spacing:</source>
        <translation type="obsolete">Інтервал мі&amp;ж рядками:</translation>
    </message>
    <message>
        <source>Abo&amp;ve:</source>
        <translation type="obsolete">Н&amp;ад:</translation>
    </message>
    <message>
        <source>&amp;Below:</source>
        <translation type="obsolete">&amp;Під:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Distances</source>
        <translation type="unfinished">Відстані</translation>
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
</context>
<context>
    <name>Editor</name>
    <message>
        <source>Editor</source>
        <translation>Редактор</translation>
    </message>
    <message>
        <source>Javascripts (*.js);;All Files (*)</source>
        <translation type="obsolete">Java-сценарії (*.js);;Всі файли (*)</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation>&amp;Відчинити...</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation>Записати &amp;як...</translation>
    </message>
    <message>
        <source>&amp;Save and Exit</source>
        <translation>&amp;Записати і вийти</translation>
    </message>
    <message>
        <source>&amp;Exit without Saving</source>
        <translation>&amp;Вийти без запису</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Відмінити</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Повторити</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Ви&amp;різати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Вклеїти</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистити</translation>
    </message>
    <message>
        <source>&amp;Get Field Names</source>
        <translation>&amp;Визначити назви полів</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагування</translation>
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
        <translation type="unfinished">Колір:</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="unfinished">Тінь:</translation>
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
        <translation type="unfinished">Негатив</translation>
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
        <translation type="unfinished">Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Вихід</translation>
    </message>
</context>
<context>
    <name>ExportForm</name>
    <message>
        <source>Choose a Export Directory</source>
        <translation>Виберіть директорію для експортування</translation>
    </message>
    <message>
        <source>&amp;All pages</source>
        <translation>&amp;Всі сторінки</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Change the output directory</source>
        <translation>Змінити вихідну директорію</translation>
    </message>
    <message>
        <source>The output directory - the place to store your images.
Name of the export file will be &apos;documentname-pagenumber.filetype&apos;</source>
        <translation>Вихідна директорія - місце в яке будуть записані Ваші
зображення. Назва експортованого файла буде у формі
&apos;імядокумента-номерсторінки.типфайла&apos;</translation>
    </message>
    <message>
        <source>Export only the current page</source>
        <translation>Експортувати лише активну сторінку</translation>
    </message>
    <message>
        <source>Available export formats</source>
        <translation>Доступні формати експортування</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>&amp;Export to Directory:</source>
        <translation>&amp;Еспортувати в директорію:</translation>
    </message>
    <message>
        <source>Image &amp;Type:</source>
        <translation>Тип &amp;зображення:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation>&amp;Якість:</translation>
    </message>
    <message>
        <source>Export as Image(s)</source>
        <translation>Експортувати, як зображення</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Установки</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation>&amp;Розрішення:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation>тнд</translation>
    </message>
    <message>
        <source>Range</source>
        <translation>Діапазон</translation>
    </message>
    <message>
        <source>&amp;Current page</source>
        <translation>Активна &amp;сторінка</translation>
    </message>
    <message>
        <source>&amp;Range</source>
        <translation>&amp;Діапазон</translation>
    </message>
    <message>
        <source>C</source>
        <translation>С</translation>
    </message>
    <message>
        <source>Export a range of pages</source>
        <translation>Експортувати діапазон сторінок</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation>Вмістити список елементів, розділених комами, де елемент
може бути * для всіх сторінок, 1-5 для діапазона сторінок, або
номер окремої сторінки.</translation>
    </message>
    <message>
        <source>Export all pages</source>
        <translation>Експортувати всі сторінки</translation>
    </message>
    <message>
        <source>Resolution of the Images
Use 72 dpi for Images intended for the Screen</source>
        <translation>Розрішення зображень
Використовуйте 72 тнд для зображень, 
призначених для виводу на екран</translation>
    </message>
    <message>
        <source>The quality of your images - 100% is the best, 1% the lowest quality</source>
        <translation>Якість зображень - 100% - найвища, 1% - найнижча якість</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Розмір:</translation>
    </message>
    <message>
        <source>Size of the images. 100% for no changes, 200% for two times larger etc.</source>
        <translation>Розмір зображень. 100% - без зміни, 200% для подвійного збільшення, і.т.д.</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Насичення</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Колір</translation>
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
        <translation type="unfinished">Розтавання</translation>
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
        <translation type="unfinished">Непрозорість:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Ім&apos;я</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Фон</translation>
    </message>
    <message>
        <source>Layers</source>
        <translation type="unfinished">Плани</translation>
    </message>
    <message>
        <source>Don&apos;t use any Path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Paths</source>
        <translation type="unfinished">Шляхи</translation>
    </message>
</context>
<context>
    <name>FDialogPreview</name>
    <message>
        <source>Size:</source>
        <translation>Розмір:</translation>
    </message>
    <message>
        <source>Title:</source>
        <translation>Заголовок:</translation>
    </message>
    <message>
        <source>No Title</source>
        <translation>Немає заголовка</translation>
    </message>
    <message>
        <source>Author:</source>
        <translation>Автор:</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Невідомий</translation>
    </message>
    <message>
        <source>Scribus Document</source>
        <translation>Документ Scribus</translation>
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
        <translation>Кольори</translation>
    </message>
    <message>
        <source>Color Sets</source>
        <translation>Набори кольорів</translation>
    </message>
    <message>
        <source>Current Color Set:</source>
        <translation>Активний набір кольорів:</translation>
    </message>
    <message>
        <source>Choose a Name</source>
        <translation>Вибрати ім&apos;я</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>New Color</source>
        <translation>Новий колір</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копія %1</translation>
    </message>
    <message>
        <source>Choose a color set to load</source>
        <translation>Вибрати набір кольорів для завантаження</translation>
    </message>
    <message>
        <source>Save the current color set</source>
        <translation>Записати активний набір кольорів</translation>
    </message>
    <message>
        <source>Remove unused colors from current document&apos;s color set</source>
        <translation>Видалити невикористані кольори з набору кольорів активного документа</translation>
    </message>
    <message>
        <source>Append colors to the current set from an existing document</source>
        <translation type="obsolete">Добавити кольори з існуючого документа в активний набір</translation>
    </message>
    <message>
        <source>Create a new color within the current set</source>
        <translation>Створити новий колір всередині активного набору</translation>
    </message>
    <message>
        <source>Edit the currently selected color</source>
        <translation>Редагувати вибраний колір</translation>
    </message>
    <message>
        <source>Make a copy of the currently selected color</source>
        <translation>Зробити копію вибраного кольору</translation>
    </message>
    <message>
        <source>Delete the currently selected color</source>
        <translation>Видалити вибраний колір</translation>
    </message>
    <message>
        <source>Make the current colorset the default color set</source>
        <translation>Використовувати активний набір кольорів, як основний</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Добавити</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагування</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Remove Unused</source>
        <translation>Видалити &amp;невикористані</translation>
    </message>
    <message>
        <source>&amp;Save Color Set</source>
        <translation>Зберегти &amp;набір кольорів</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation>&amp;Ім&apos;я:</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Нічого</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">І&amp;мпортувати</translation>
    </message>
    <message>
        <source>Import colors to the current set from an existing document</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPrefs</name>
    <message>
        <source>Global Font Settings</source>
        <translation type="obsolete">Глобальнi установки шрифтiв</translation>
    </message>
    <message>
        <source>Available Fonts</source>
        <translation>Доступні шрифти</translation>
    </message>
    <message>
        <source>Font Substitutions</source>
        <translation>Підміна шрифтів</translation>
    </message>
    <message>
        <source>Additional Paths</source>
        <translation>Додаткові шляхи</translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Постскрипт</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Так</translation>
    </message>
    <message>
        <source>Font Name</source>
        <translation>Ім&apos;я шрифта</translation>
    </message>
    <message>
        <source>Replacement</source>
        <translation>Підміна</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Вибрати директорію</translation>
    </message>
    <message>
        <source>Use Font</source>
        <translation type="obsolete">Використовувати</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <translation type="obsolete">Вбудувати в:</translation>
    </message>
    <message>
        <source>Subset</source>
        <translation type="obsolete">Вибірка</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Тип</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <translation type="obsolete">Шлях до файла шрифта</translation>
    </message>
    <message>
        <source>&amp;Available Fonts</source>
        <translation>&amp;Доступні шрифти</translation>
    </message>
    <message>
        <source>Font &amp;Substitutions</source>
        <translation>&amp;Підміна шрифтів</translation>
    </message>
    <message>
        <source>Additional &amp;Paths</source>
        <translation>Д&amp;одаткові шляхи до шрифтів</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>A&amp;dd...</source>
        <translation>&amp;Додати...</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Ви&amp;далити</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Ім&apos;я шрифта</translation>
    </message>
    <message>
        <source>Use Font</source>
        <comment>font preview</comment>
        <translation type="unfinished">Використовувати</translation>
    </message>
    <message>
        <source>Embed in:</source>
        <comment>font preview</comment>
        <translation type="unfinished">Вбудувати в:</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Вибірка</translation>
    </message>
    <message>
        <source>Path to Font File</source>
        <comment>font preview</comment>
        <translation type="unfinished">Шлях до файла шрифта</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Font search paths can only be set in Preferences, and only when there is no document currently open. Close any open documents, then use Edit-&gt;Settings to change the font search path.&lt;/qt&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FontPreview</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Реве та стогне Дніпр широкий, сердитий вітер завива</translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <translation type="obsolete">Попередній перегляд шрифтів</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <translation type="obsolete">Додати вибраний шрифт в меню Стиль, Шрифт</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <translation type="obsolete">Вихід з попереднього перегляду</translation>
    </message>
    <message>
        <source>Size of the Font</source>
        <translation type="obsolete">Попередній перегляд шрифта</translation>
    </message>
    <message>
        <source>Font Name</source>
        <comment>font preview</comment>
        <translation type="unfinished">Ім&apos;я шрифта</translation>
    </message>
    <message>
        <source>Doc</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type</source>
        <comment>font preview</comment>
        <translation type="unfinished">Тип</translation>
    </message>
    <message>
        <source>Subset</source>
        <comment>font preview</comment>
        <translation type="unfinished">Вибірка</translation>
    </message>
    <message>
        <source>Access</source>
        <comment>font preview</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Font Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Fonts Preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Попередній перегляд шрифтів</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <source>Quick Search: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation type="unfinished">&amp;Пошук</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <comment>font preview</comment>
        <translation type="unfinished">&amp;Зачинити</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <comment>font preview</comment>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>Append selected font into Style, Font menu</source>
        <comment>font preview</comment>
        <translation type="unfinished">Додати вибраний шрифт в меню Стиль, Шрифт</translation>
    </message>
    <message>
        <source>Leave preview</source>
        <comment>font preview</comment>
        <translation type="unfinished">Вихід з попереднього перегляду</translation>
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
        <translation type="unfinished">Реве та стогне Дніпр широкий, сердитий вітер завива</translation>
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
        <translation type="unfinished">&amp;Добавити</translation>
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
        <translation>Позиція:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Here you can add, change or remove Color-Stops.</source>
        <translation>Тут Ви можете добавити, змінити або видалити переходи кольорів.</translation>
    </message>
</context>
<context>
    <name>GuideManager</name>
    <message>
        <source>Manage Guides</source>
        <translation>Керування направляючими</translation>
    </message>
    <message>
        <source>Horizontal Guides</source>
        <translation>Горизонтальні направляючі</translation>
    </message>
    <message>
        <source>Vertical Guides</source>
        <translation>Вертикальні направляючі</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Поз. &amp;У:</translation>
    </message>
    <message>
        <source>&amp;Add</source>
        <translation>&amp;Додати</translation>
    </message>
    <message>
        <source>D&amp;elete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Поз. &amp;Х:</translation>
    </message>
    <message>
        <source>A&amp;dd</source>
        <translation>&amp;Додати</translation>
    </message>
    <message>
        <source>De&amp;lete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Lock Guides</source>
        <translation>&amp;Замкнути направляючі</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
        <translation type="unfinished">С&amp;торінка</translation>
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
        <translation type="unfinished">&amp;Зачинити</translation>
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
</context>
<context>
    <name>HelpBrowser</name>
    <message>
        <source>Sorry, no manual available! Please see: http://docs.scribus.net for updated docs
and www.scribus.net for downloads.</source>
        <translation>Вибачте - довідка відсутня! Будь-ласка зверніться на http://docs.scribus.net за поновленою документацією та yf ww.scribus.net для зкачування.</translation>
    </message>
    <message>
        <source>Contents</source>
        <translation>Зміст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ссилка</translation>
    </message>
    <message>
        <source>Scribus Online Help</source>
        <translation>Допомога по Scribus на вебі</translation>
    </message>
    <message>
        <source>&amp;Contents</source>
        <translation>&amp;Зміст</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Пошук</translation>
    </message>
    <message>
        <source>S&amp;earch</source>
        <translation type="obsolete">П&amp;ошук</translation>
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
        <translation>невідоме</translation>
    </message>
    <message>
        <source>Find</source>
        <translation>Шукати</translation>
    </message>
    <message>
        <source>Search Term:</source>
        <translation>Термін для пошуку:</translation>
    </message>
    <message>
        <source>Se&amp;arch</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="unfinished">&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
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
        <translation type="unfinished">&amp;Друк...</translation>
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
        <translation type="unfinished">&amp;Файл</translation>
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
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Закладки</translation>
    </message>
</context>
<context>
    <name>HyAsk</name>
    <message>
        <source>Possible Hyphenation</source>
        <translation>Можливий перенос</translation>
    </message>
    <message>
        <source>Accept</source>
        <translation>Застосувати</translation>
    </message>
    <message>
        <source>Skip</source>
        <translation>Пропустити</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
</context>
<context>
    <name>HySettings</name>
    <message>
        <source>Hyphenator Settings</source>
        <translation type="obsolete">Установки переносу</translation>
    </message>
    <message>
        <source>Length of the smallest word to be hyphenated.</source>
        <translation>Довжина найкоротшого слова, яке буде розділене для переносу.</translation>
    </message>
    <message>
        <source>Maximum number of Hyphenations following each other.
A value of 0 means unlimited hyphenations.</source>
        <translation>Максимальне число послідовних переносів.
Нульове значення встановлює необмежену кількість
переносів.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation>&amp;Мова:</translation>
    </message>
    <message>
        <source>&amp;Smallest Word:</source>
        <translation>&amp;Найменше слово:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Hyphenation Suggestions</source>
        <translation>&amp;Пропозиції по переносу</translation>
    </message>
    <message>
        <source>Hyphenate Text Automatically &amp;During Typing</source>
        <translation>Переносити текст автоматично під &amp;час друкування</translation>
    </message>
    <message>
        <source>A dialog box showing all possible hyphens for each word will show up when you use the Extras, Hyphenate Text option.</source>
        <translation>Діалогове вікно, яке показує всі можливі переноси для кожного слова, буде показане якщо Ви виберете установку &quot;Додаткове, Переносити текст&quot;.</translation>
    </message>
    <message>
        <source>Enables automatic hyphenation of your text while typing.</source>
        <translation>Ввімкнути автоматичний переніс тексту під час друкування.</translation>
    </message>
    <message>
        <source>Consecutive Hyphenations &amp;Allowed:</source>
        <translation>&amp;Дозволити послідовні переноси:</translation>
    </message>
</context>
<context>
    <name>ImportPSPlugin</name>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="unfinished">Імпорт &amp;ЕPS/PS...</translation>
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
</context>
<context>
    <name>InsPage</name>
    <message>
        <source>Insert Page</source>
        <translation>Вставити сторінку</translation>
    </message>
    <message>
        <source>Inserting</source>
        <translation type="obsolete">Вставка</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation>перед цією сторінкою</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation>після цієї сторінки</translation>
    </message>
    <message>
        <source>at End</source>
        <translation>в кінці документа</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Звичайний</translation>
    </message>
    <message>
        <source>Template (Right Page):</source>
        <translation type="obsolete">Шаблон (Права сторінка):</translation>
    </message>
    <message>
        <source>&amp;Inserting</source>
        <translation type="obsolete">&amp;Вставка</translation>
    </message>
    <message>
        <source>Page(s)</source>
        <translation>Сторінка(и)</translation>
    </message>
    <message>
        <source>&amp;Template (Left Page):</source>
        <translation type="obsolete">Шаблон (&amp;Ліва сторінка):</translation>
    </message>
    <message>
        <source>&amp;Template:</source>
        <translation type="obsolete">&amp;Шаблон:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="unfinished">&amp;Вставити</translation>
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
        <translation type="unfinished">Розмір сторінки</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Розмір:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Нестандартний</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Орієнтація:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Горизонтальна</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Висота:</translation>
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
        <translation>Вставити таблицю</translation>
    </message>
    <message>
        <source>Number of Rows:</source>
        <translation type="obsolete">Число рядків:</translation>
    </message>
    <message>
        <source>Number of Columns:</source>
        <translation type="obsolete">Число стовпців:</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
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
        <source>New Script</source>
        <translation>Новий сценарій</translation>
    </message>
    <message>
        <source>Edit JavaScripts</source>
        <translation>Редагувати сценарії JavaScript</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation>&amp;Редагувати...</translation>
    </message>
    <message>
        <source>&amp;Add...</source>
        <translation>&amp;Додати...</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Зачинити</translation>
    </message>
    <message>
        <source>&amp;New Script:</source>
        <translation>&amp;Новий сценарій:</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ні</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Так</translation>
    </message>
    <message>
        <source>Do you really want to delete this Script?</source>
        <translation>Ви справді бажаєте видалити цей сценарій?</translation>
    </message>
</context>
<context>
    <name>KeyManager</name>
    <message>
        <source>Manage Keyboard Shortcuts</source>
        <translation type="obsolete">Керування гарячими клавішами</translation>
    </message>
    <message>
        <source>Action</source>
        <translation>Дія</translation>
    </message>
    <message>
        <source>Current Key</source>
        <translation>Активна клавіша</translation>
    </message>
    <message>
        <source>Select a Key for this Action</source>
        <translation>Вибрати клавішу для цієї дії</translation>
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
        <translation>Застереження</translation>
    </message>
    <message>
        <source>&amp;No Key</source>
        <translation>&amp;Без ключа</translation>
    </message>
    <message>
        <source>&amp;User Defined Key</source>
        <translation>Ключ визначений &amp;користувачем</translation>
    </message>
    <message>
        <source>Set &amp;Key</source>
        <translation>&amp;Установити ключ</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>This Key Sequence is already in use</source>
        <translation>Ця послідовність ключа уже використана</translation>
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
</context>
<context>
    <name>LayerPalette</name>
    <message>
        <source>Layers</source>
        <translation>Плани</translation>
    </message>
    <message>
        <source>Add a new Layer</source>
        <translation type="obsolete">Додати новий план</translation>
    </message>
    <message>
        <source>Delete Layer</source>
        <translation>Видалити план</translation>
    </message>
    <message>
        <source>Raise Layer</source>
        <translation type="obsolete">Підняти план</translation>
    </message>
    <message>
        <source>Lower Layer</source>
        <translation type="obsolete">Опустити план</translation>
    </message>
    <message>
        <source>New Layer</source>
        <translation type="obsolete">Новий план</translation>
    </message>
    <message>
        <source>Do you want to delete all Objects on this Layer too?</source>
        <translation type="obsolete">Ви бажаєте видалити всі об&apos;екти разом з цим планом?</translation>
    </message>
    <message>
        <source>Do you want to delete all objects on this layer too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Ім&apos;я</translation>
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
        <translation>Редагування стилів ліній</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копія %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Новий стиль</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Добавити</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагувати</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Записати</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ні</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Так</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation>Ви справді бажаєте видалити цей стиль?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">І&amp;мпортувати</translation>
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
        <translation type="unfinished">Автор:</translation>
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
        <translation type="unfinished">Alt+O</translation>
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
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
</context>
<context>
    <name>Macro</name>
    <message>
        <source>Passed object is not callable</source>
        <comment>python error</comment>
        <translation type="obsolete">Переданий об&apos;єкт не є виконуваним</translation>
    </message>
</context>
<context>
    <name>MacroManager</name>
    <message>
        <source>Manage Macros</source>
        <translation type="obsolete">Керувати макросами</translation>
    </message>
    <message>
        <source>Brings up a graphical window for creating, deleting, editing, saving and loading macros.</source>
        <translation type="obsolete">Відчиняє графічне вікно для створення, видалення, редагування, запису та завантаження макросів.</translation>
    </message>
    <message>
        <source>Create, edit and delete macros</source>
        <translation type="obsolete">Створити, редагувати та видаляти макроси</translation>
    </message>
    <message>
        <source>&amp;Macro</source>
        <translation type="obsolete">&amp;Макрос</translation>
    </message>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Менеджер макросів</translation>
    </message>
    <message>
        <source>Unable to open the requested file: %1</source>
        <translation type="obsolete">Неможливо відчинити вказаний файл: %1</translation>
    </message>
    <message>
        <source>Scribus - Edit Macro</source>
        <translation type="obsolete">Scribus - Редагувати макрос</translation>
    </message>
    <message>
        <source>&lt;qt&gt;The macro name you requested is already taken  by another macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Вибране ім&apos;я макроса вже було використане для іншого макроса.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Macro creation failed. The macro manager was unable to set up the macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Створення макроса не було успішним. Менеджер макросів не зміг ініціалізувати макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; has reported a minor error.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Макрос &apos;%1&apos; звітує про невелику помилку.
Помилка: %2
Повний текст помилки слідує:

%3
</translation>
    </message>
    <message>
        <source>The macro &apos;%1&apos; failed to execute correctly.
The error is: %2
A full traceback follows:

%3
</source>
        <translation type="obsolete">Макрос &apos;%1&apos; не міг бути успішно виконаним.
Помилка: %2
Повний текст помилки слідує:

%3
</translation>
    </message>
    <message>
        <source>Scribus - New Macro</source>
        <translation type="obsolete">Scribus - Новий макрос</translation>
    </message>
    <message>
        <source>Enter name for new macro: </source>
        <translation type="obsolete">Введіть ім&apos;я нового макроса:</translation>
    </message>
</context>
<context>
    <name>ManageMacrosDialog</name>
    <message>
        <source>Scribus - Macro Manager</source>
        <translation type="obsolete">Scribus - Менеджер макросів</translation>
    </message>
    <message>
        <source>Renaming the macro failed because the name is already in use.</source>
        <translation type="obsolete">Перейменування макроса не було успішним, тому що вибране ім&apos;я вже зайняте.</translation>
    </message>
    <message>
        <source>Scribus - Manage Macros</source>
        <translation type="obsolete">Scribus - Керувати макросами</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;This window is the Scribus Macro Manager. Here you can create macros, edit macros, etc. All changes are made using the buttons on the right hand side of the window.&lt;/p&gt;
&lt;p&gt;All changes made in this dialog take effect instantly - you cannot cancel the actions you make here.
The table in the center of the dialog lists what macros are currently loaded and some information about them. Use &quot;What&apos;s this&quot; on the table for more information.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Це вікно менеджера макросів Scribus. Тут Ви можете створювати макроси, редагувати макроси і т.д. Всі зміни виконуються за допомогою кнопок з правої сторони вікна.&lt;/p&gt;
&lt;p&gt;Всі зміни зроблені в цьому діалоговому вікні виконуютья миттєво - Ви не можете відмінити Ваші дії. Використовуйте &quot;Що це&quot; на таблиці для отримання додаткової інформації.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Новий</translation>
    </message>
    <message>
        <source>Alt+N</source>
        <translation type="obsolete">Alt+N</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Створити новий макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Create a new macro by prompting for the macro name then bringing up the edit macro dialog box.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Створити новий макрос - задати ім&apos;я макроса і відчинити діалогове вікно редагування макроса.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Macro</source>
        <translation type="obsolete">Макрос</translation>
    </message>
    <message>
        <source>Edit</source>
        <translation type="obsolete">Редагувати</translation>
    </message>
    <message>
        <source>Accel</source>
        <translation type="obsolete">Пришвидшувач</translation>
    </message>
    <message>
        <source>Description</source>
        <translation type="obsolete">Опис</translation>
    </message>
    <message>
        <source>&lt;p&gt;This table lists the macros that are currently defined.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Name:&lt;/b&gt; The name of the macro, as shown in the menu bar and in other places around Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Edit:&lt;/b&gt; If the macro can be edited, &quot;Yes&quot; appears in this column. Usually if a macro cannot be edited it was created using the register_macro command in a script.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Accel:&lt;/b&gt; The menu shortcut key sequence, if any, associated with the macro. For example, CTRL-F8 means that you can press Control-F8 when in Scribus to run the macro.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Description:&lt;/b&gt; If the macro contains a &quot;docstring&quot;, a special string at the start of its definition that describes it, that is shown here. If the docstring is long, only the beginning is shown - use &quot;What&apos;s This&quot; on the macro&apos;s entry in the Macro menu to see the full description.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Ця таблиця містить попередньо визначені макроси.&lt;/p&gt;

&lt;p&gt;&lt;b&gt;Ім&apos;я:&lt;/b&gt; Ім&apos;я макроса так, як воно показується в меню та в інших частинах Scribus.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Ред.:&lt;/b&gt; Показує чи макрос може бути редагованим. В цій колонці повинне бути слово &quot;Так&quot;. Якщо макрос не може бути редагованим, це звичайно означає, що він був створений командою register_macro в сценарії.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Пришвидш.:&lt;/b&gt; Гаряча комбінація клавіш для пункта меню, яка зв&apos;язана з макросом. Наприклад, CTRL-F8 означає, що Ви можете натиснути Control-F8, щоб запустити макрос.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;Опис:&lt;/b&gt; Якщо макрос містить &quot;пояснення&quot; - спеціальну строкову величину на початку сценарія, вона буде тут показана. Якщо &quot;пояснення&quot; дуже довге - буде показано лише його початок. Використовуйте інструмент &quot;Що це&quot; в меню Макросів для доступу до повного опису певного макроса.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Rena&amp;me</source>
        <translation type="obsolete">&amp;Перейменувати</translation>
    </message>
    <message>
        <source>Alt+M</source>
        <translation type="obsolete">Alt+M</translation>
    </message>
    <message>
        <source>Rename the selected macro.</source>
        <translation type="obsolete">Перейменувати вибраний макрос.</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Rename the selected macro. You will be prompted for the new name.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Перейменувати вибраний макрос. Вас буде запитано про нове ім&apos;я.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Edit...</source>
        <translation type="obsolete">&amp;Редагувати...</translation>
    </message>
    <message>
        <source>Alt+E</source>
        <translation type="obsolete">Alt+E</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Edit the source of the selected macro, if the source is availible.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Редагувати вихідний код вибраного макроса, якщо він є в наявності.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Видалити</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="obsolete">Alt+D</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Delete the currently selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Видалити вибраний макрос.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete the selected macro. This is instant, and there is no way to recover the macro once deleted. If the macro is created by a start-up script, it will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Видалити вибраний макрос. Ця операція виконується відразу і відновити видалений макрос буде неможливо. Якщо макрос був створений сценарієм, що виконується при запуску Scribus він з&apos;явиться при наступному запуску Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Set Accel</source>
        <translation type="obsolete">&amp;Уст. Гар. Клавішу</translation>
    </message>
    <message>
        <source>Alt+S</source>
        <translation type="obsolete">Alt+S</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Set the keyboard shortcut for the selected macro.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Установити гарячу комбінацію клавіш для вибраного макроса.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;p&gt;Set the keyboard shortcut (accelerator) key of the selected macro. You will be prompted for the new shortcut in a dialog box.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Установлює гарячу комбінацію клавіш для вибраного макроса. Вас буде запитано про нову комбінацію за допомогою діалогового вікна.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>E&amp;xport</source>
        <translation type="obsolete">Е&amp;кспорт</translation>
    </message>
    <message>
        <source>Alt+X</source>
        <translation type="obsolete">Alt+X</translation>
    </message>
    <message>
        <source>Export macros to a file.</source>
        <translation type="obsolete">Експортувати макрос в файл.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Export macros to an external file. The file will be a Python script containing the scripter commands to re-create the macros. It can be run using &lt;tt&gt;Load extension script&lt;/tt&gt; from the &lt;tt&gt;Script&lt;/tt&gt; menu, or the import button in the macro manager.&lt;/p&gt;
&lt;p&gt;If you want a nice, human readable version of your macros, select the macro you want, press the &lt;tt&gt;Edit&lt;/tt&gt;  button, and use the &lt;tt&gt;Save source&lt;/tt&gt; button in the &lt;tt&gt;Edit Macro&lt;/tt&gt; dialog. You won&apos;t be able to load that version with &lt;tt&gt;Load extension script&lt;/tt&gt; - instead, create a new macro with the&lt;tt&gt; New&lt;/tt&gt; button and use &lt;tt&gt;Load source&lt;/tt&gt;.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Експортувати макроси у зовнішній файл. Цей файл буде сценарієм Пітона, який буде містити команди сценариста для відтворення макросів. Він може бути виконаний за допомогою &lt;tt&gt;Завантажити додатковий сценарій&lt;/tt&gt; з меню &lt;tt&gt;Сценарії&lt;/tt&gt; або за допомогою кнопки імпорту в менеджері макросів.&lt;/p&gt;
&lt;p&gt;Якщо Ви бажаєте отримати версію макросів форматовану для перегляду людиною, виберіть макрос і натисніть на кнопку &lt;tt&gt;Редагувати&lt;/tt&gt;  та використайте кнопку &lt;tt&gt;Записати вихідний код&lt;/tt&gt; в діалоговому вікні &lt;tt&gt;Редагувати Макрос&lt;/tt&gt;. Ви не зможете завантажити отриманий варіант макроса через &lt;tt&gt;Завантажити додатковий сценарій&lt;/tt&gt;. Натомість створіть новий макрос кнопкою &lt;tt&gt; Новий&lt;/tt&gt; та використайте &lt;tt&gt;Завантажити вихідний код&lt;/tt&gt;.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Delete &amp;All</source>
        <translation type="obsolete">Видалити &amp;Все</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="obsolete">Alt+A</translation>
    </message>
    <message>
        <source>Delete all macros.</source>
        <translation type="obsolete">Видалити всі макроси.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Delete all registered macros. This is instant, and there is no way to recover the deleted macros. Any macros created by your start-up script will reappear next time you load Scribus.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Видалити всі зареєстровані макроси. Ця операція виконується негайно і метода відновлення видалених макросів не існує. Любі макроси створені сценарієм, який виконується при запуску Scribus, будуть відновлені при наступному запуску Scribus.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="obsolete">І&amp;мпортувати</translation>
    </message>
    <message>
        <source>Alt+I</source>
        <translation type="obsolete">Alt+I</translation>
    </message>
    <message>
        <source>Import macros from a file.</source>
        <translation type="obsolete">Імпортувати макроси з файла.</translation>
    </message>
    <message>
        <source>&lt;p&gt;Loads macros from an external file.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Імпортувати макроси із зовнішнього файла.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Close this dialog</source>
        <translation type="obsolete">Зачинити це діалогове вікно</translation>
    </message>
    <message>
        <source>Return to Scribus</source>
        <translation type="obsolete">Повернутися в Scribus </translation>
    </message>
    <message>
        <source>&lt;p&gt;Edit the selected macro. &lt;/p&gt;
&lt;p&gt;If this button is greyed out, either there is no selected macro or the macro manager does not have the source code for the macro you have selected (in which case &lt;tt&gt;No&lt;/tt&gt; will be shown in the &lt;tt&gt;Edit &lt;/tt&gt;column of the macro).&lt;/p&gt;
&lt;p&gt;If Scribus doesn&apos;t have the source, the macro was probably created by a script.&lt;/p&gt;</source>
        <translation type="obsolete">&lt;p&gt;Редагувати вибраний макрос.&lt;/p&gt;
&lt;p&gt;Якщо ця кнопка виділена сірим кольоров то Ви або не вибрали жодного макроса або менеджер макросів не містить вихідний код для вибраного макроса (в цьому випадку &lt;tt&gt;Ні&lt;/tt&gt; буде показано в колонці макроса &lt;tt&gt;Редагувати&lt;/tt&gt;).&lt;/p&gt;
&lt;p&gt;Якщо в Scribus немає вихідного кода макроса то він, скоріше всього, був створений сценарієм.&lt;/p&gt;</translation>
    </message>
    <message>
        <source>Scribus Macro Files (*.pymacro)</source>
        <translation type="obsolete">Файли Макросів Scribus (*.pymacro)</translation>
    </message>
    <message>
        <source>Open File Dialog</source>
        <translation type="obsolete">Діалого вікно Відчинити Файл</translation>
    </message>
    <message>
        <source>Select the macro file to load.</source>
        <translation type="obsolete">Виберіть файл з макросом для завантаження.</translation>
    </message>
    <message>
        <source>Save File Dialog</source>
        <translation type="obsolete">Діалогове вікно Записати Файл</translation>
    </message>
    <message>
        <source>Save all macros</source>
        <translation type="obsolete">Записати всі макроси</translation>
    </message>
    <message>
        <source>Scribus - Rename Macro</source>
        <translation type="obsolete">Scribus - Перейменувати макрос</translation>
    </message>
    <message>
        <source>Enter new name: </source>
        <translation type="obsolete">Введіть нове ім&apos;я :</translation>
    </message>
    <message>
        <source>Scribus - Set Macro Shortcut</source>
        <translation type="obsolete">Scribus - Установити гарячу комбінацію клавіш для макроса</translation>
    </message>
    <message>
        <source>Enter new shortcut: </source>
        <translation type="obsolete">Введіть нову комбінацію клавіш:</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Так</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ні</translation>
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
        <translation type="unfinished">Розмір сторінки</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Розмір:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Нестандартний</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Орієнтація:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation type="unfinished">Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation type="unfinished">Горизонтальна</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Висота:</translation>
    </message>
    <message>
        <source>Move Objects with their Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Type:</source>
        <translation type="unfinished">Тип:</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation type="unfinished">Розмітка полів</translation>
    </message>
</context>
<context>
    <name>MarginWidget</name>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="unfinished">&amp;Низ:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="unfinished">&amp;Верх:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="unfinished">&amp;Правий край:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="unfinished">&amp;Лівий край:</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="unfinished">Відстань від границі верхнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="unfinished">Відстань від границі нижнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="unfinished">&amp;Зсередини:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="unfinished">Зз&amp;овні:</translation>
    </message>
    <message>
        <source>Preset Layouts:</source>
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
        <source>Duplicates the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Deletes the selected master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Adds a new master page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports master pages from another document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Застереження</translation>
    </message>
    <message>
        <source>Do you really want to delete this master page?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="unfinished">&amp;Ні</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="unfinished">&amp;Так</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="unfinished">&amp;Ім&apos;я:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="unfinished">Копія %1</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Ім&apos;я:</translation>
    </message>
    <message>
        <source>New MasterPage</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Копія №%1 з </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Mdup</name>
    <message>
        <source>Multiple Duplicate</source>
        <translation>Множинне дублювання</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">пт</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>&amp;Number of Copies:</source>
        <translation>&amp;Число копій:</translation>
    </message>
    <message>
        <source>&amp;Horizontal Shift:</source>
        <translation>&amp;Горизонтальний зсув:</translation>
    </message>
    <message>
        <source>&amp;Vertical Shift:</source>
        <translation>&amp;Вертикальний зсув:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>Measurements</name>
    <message>
        <source>Distances</source>
        <translation>Відстані</translation>
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
        <translation>Кут:</translation>
    </message>
    <message>
        <source>Length:</source>
        <translation>Довжина:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="unfinished">тчк</translation>
    </message>
</context>
<context>
    <name>MenuTest</name>
    <message>
        <source>Script error</source>
        <translation type="obsolete">Помилка сценарія</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation type="obsolete">Якщо ви використовуєте офіційний сценарій то, будь-ласка, повідомляйте про помилки на &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Показати &amp;Консоль</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Сховати &amp;Консоль</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation type="obsolete">Це повідомлення також розміщене в блоці копіювання. Використайте 
Ctrl-V для вставки його в систему обробітку програмних помилок.</translation>
    </message>
</context>
<context>
    <name>MergeDoc</name>
    <message>
        <source>Change...</source>
        <translation type="obsolete">Вибрати...</translation>
    </message>
    <message>
        <source>Import</source>
        <translation type="obsolete">Імпортувати</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Import Template</source>
        <translation type="obsolete">Шаблон імпортування</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Імпортувати сторінку(и)</translation>
    </message>
    <message>
        <source>From Document:</source>
        <translation type="obsolete">З документа:</translation>
    </message>
    <message>
        <source>Import Page(s):</source>
        <translation type="obsolete">Імпортувати сторінку(и):</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Вмістити список елементів, розділений комами де елемент
може *бути для всіх сторінок, 1-5 для діапазона сторінок, або
номер окремої сторінки.</translation>
    </message>
    <message>
        <source> from 0</source>
        <translation>від 0</translation>
    </message>
    <message>
        <source>Create Page(s)</source>
        <translation>Створити Сторінку(и)</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">перед сторінкою</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">після сторінки</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">в кінці</translation>
    </message>
    <message>
        <source> from %1</source>
        <translation>з %1</translation>
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
        <translation type="unfinished">І&amp;мпортувати</translation>
    </message>
</context>
<context>
    <name>MissingFont</name>
    <message>
        <source>Missing Font</source>
        <translation type="unfinished">Відсутній шрифт</translation>
    </message>
    <message>
        <source>The Font %1 is not installed.</source>
        <translation type="unfinished">Шрифт %1 не встановлений.</translation>
    </message>
    <message>
        <source>Use</source>
        <translation type="unfinished">Використати</translation>
    </message>
    <message>
        <source>instead</source>
        <translation type="unfinished">замість</translation>
    </message>
</context>
<context>
    <name>MovePages</name>
    <message>
        <source>Move Pages</source>
        <translation>Переміщення сторінок</translation>
    </message>
    <message>
        <source>Copy Page</source>
        <translation>Скопіювати сторінку</translation>
    </message>
    <message>
        <source>Move Page(s):</source>
        <translation>Перемістити сторінку(и):</translation>
    </message>
    <message>
        <source>to:</source>
        <translation>до:</translation>
    </message>
    <message>
        <source>before Page</source>
        <translation type="obsolete">перед сторінкою</translation>
    </message>
    <message>
        <source>after Page</source>
        <translation type="obsolete">за сторінку</translation>
    </message>
    <message>
        <source>at End</source>
        <translation type="obsolete">в кінець документа</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
</context>
<context>
    <name>Mpalette</name>
    <message>
        <source>Properties</source>
        <translation>Властивості</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ім&apos;я</translation>
    </message>
    <message>
        <source>Geometry</source>
        <translation>Геометрія</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>Basepoint:</source>
        <translation>Точка відліку:</translation>
    </message>
    <message>
        <source>Level</source>
        <translation>План</translation>
    </message>
    <message>
        <source>Shape:</source>
        <translation>Фігура:</translation>
    </message>
    <message>
        <source>Distance of Text</source>
        <translation>Відстань від тексту</translation>
    </message>
    <message>
        <source>Show Curve</source>
        <translation>Показати криву</translation>
    </message>
    <message>
        <source>Start Offset:</source>
        <translation>Почати відступ:</translation>
    </message>
    <message>
        <source>Distance from Curve:</source>
        <translation>Відстань від кривої:</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Custom Spacing</source>
        <translation type="obsolete">Нестандартний проміжок</translation>
    </message>
    <message>
        <source>Input Profile:</source>
        <translation>Профіль вводу:</translation>
    </message>
    <message>
        <source>Rendering Intent:</source>
        <translation>Схема перерахунку кольорів:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation>Уявний</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation>Відносна кольорометрія</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation>Насичення</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation>Абсолютна кольорометрія</translation>
    </message>
    <message>
        <source>Left Point</source>
        <translation>Ліва точка</translation>
    </message>
    <message>
        <source>End Points</source>
        <translation>Кінцеві точки</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Фацеточне з&apos;єднання</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Фасочне з&apos;єднання</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Заокруглене з&apos;єднання</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Пласка верхівка</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратна верхівка</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Заокруглена верхівка</translation>
    </message>
    <message>
        <source>No Style</source>
        <translation>Стиль не встановлено</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Розмір шрифта</translation>
    </message>
    <message>
        <source>Line Spacing</source>
        <translation>Проміжки між рядками</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ручна обробка</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ім&apos;я &quot;%1&quot; вже використане.
Будь-ласка виберіть інше.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Shade:</source>
        <translation type="obsolete">Тінь:</translation>
    </message>
    <message>
        <source>Name of selected object</source>
        <translation>Назва вибраного об&apos;єкту</translation>
    </message>
    <message>
        <source>Horizontal position of current basepoint</source>
        <translation>Горизонтальна позиція активної точки відліку</translation>
    </message>
    <message>
        <source>Vertical position of current basepoint</source>
        <translation>Вертикальна позиція активної точки відліку</translation>
    </message>
    <message>
        <source>Width</source>
        <translation>Ширина</translation>
    </message>
    <message>
        <source>Height</source>
        <translation>Висота</translation>
    </message>
    <message>
        <source>Rotation of object at current basepoint</source>
        <translation>Кут повороту об&apos;єкту від активної точки відліку</translation>
    </message>
    <message>
        <source>Point from which measurements or rotation angles are referenced</source>
        <translation>Точка, відносно якої вимірюються відстані та кути обертання</translation>
    </message>
    <message>
        <source>Select top left for basepoint</source>
        <translation>Вибрати верхній лівий кут, як точку відліку</translation>
    </message>
    <message>
        <source>Select top right for basepoint</source>
        <translation>Вибрати верхній правий кут, як точку відліку</translation>
    </message>
    <message>
        <source>Select bottom left for basepoint</source>
        <translation>Вибрати нижній лівий кут, як точку відліку</translation>
    </message>
    <message>
        <source>Select bottom right for basepoint</source>
        <translation>Вибрати нижній правий кут, як точку відліку</translation>
    </message>
    <message>
        <source>Select center for basepoint</source>
        <translation>Вибрати центр, як точку відліку</translation>
    </message>
    <message>
        <source>Flip Horizontal</source>
        <translation>Перевернути по горизонталі</translation>
    </message>
    <message>
        <source>Flip Vertical</source>
        <translation>Перевернути по вертикалі</translation>
    </message>
    <message>
        <source>Move one level up</source>
        <translation>Перемістити на один план вгору</translation>
    </message>
    <message>
        <source>Move one level down</source>
        <translation>Перемістити на один план вниз</translation>
    </message>
    <message>
        <source>Move to front</source>
        <translation>Перемістити на передній план</translation>
    </message>
    <message>
        <source>Move to back</source>
        <translation>Перемістити на задній план</translation>
    </message>
    <message>
        <source>Lock or unlock the object</source>
        <translation>Замкнути або відімкнути об&apos;єкт</translation>
    </message>
    <message>
        <source>Lock or unlock the size of the object</source>
        <translation>Закнути або відімкнути розмір об&apos;єкта</translation>
    </message>
    <message>
        <source>Enable or disable printing of the object</source>
        <translation>Дозволити або заборонити друкування об&apos;екта</translation>
    </message>
    <message>
        <source>Font of selected text or object</source>
        <translation>Шрифт вибраного тексту чи об&apos;єкта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Масштабування ширини знаків</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Колір знакових силуетів</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Колір заповнення знаків</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Насиченість кольору знакових силуетів</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Насиченість кольору заповнення знаків</translation>
    </message>
    <message>
        <source>Style of current paragraph</source>
        <translation>Стиль активного абзацу</translation>
    </message>
    <message>
        <source>Change settings for left or end points</source>
        <translation>Змінити установки для лівих та правих закінчень</translation>
    </message>
    <message>
        <source>Pattern of line</source>
        <translation>Вид лінії</translation>
    </message>
    <message>
        <source>Thickness of line</source>
        <translation>Товщина лінії</translation>
    </message>
    <message>
        <source>Type of line joins</source>
        <translation>Тип з&apos;єднань ліній</translation>
    </message>
    <message>
        <source>Type of line end</source>
        <translation>Тип закінчення лінії</translation>
    </message>
    <message>
        <source>Line style of current object</source>
        <translation>Стиль ліній активного об&apos;єкту</translation>
    </message>
    <message>
        <source>Choose the shape of frame...</source>
        <translation>Вибрати форму рамки...</translation>
    </message>
    <message>
        <source>Edit shape of the frame...</source>
        <translation>Редагувати форму рамки...</translation>
    </message>
    <message>
        <source>Set radius of corner rounding</source>
        <translation>Установити радіус заокруглення кутів</translation>
    </message>
    <message>
        <source>Number of columns in text frame</source>
        <translation>Число стовпців в текстовій рамці</translation>
    </message>
    <message>
        <source>Distance between columns</source>
        <translation>Інтервал між стовпцями</translation>
    </message>
    <message>
        <source>Distance of text from top of frame</source>
        <translation>Відстань від верхівки рамки до тексту </translation>
    </message>
    <message>
        <source>Distance of text from bottom of frame</source>
        <translation>Відстань від тексту до низу рамки</translation>
    </message>
    <message>
        <source>Distance of text from left of frame</source>
        <translation>Відстань від лівої сторони рамки до тексту</translation>
    </message>
    <message>
        <source>Distance of text from right of frame</source>
        <translation>Відстань від тексту до правої сторони рамки</translation>
    </message>
    <message>
        <source>Edit tab settings of text frame...</source>
        <translation>Редагувати установки відступів текстової рамки...</translation>
    </message>
    <message>
        <source>Allow the image to be a different size to the frame</source>
        <translation>Дозволити неспівпадання розмірів зображення та рамки</translation>
    </message>
    <message>
        <source>Horizontal offset of image within frame</source>
        <translation>Горизонтальне зміщення зображення в рамці</translation>
    </message>
    <message>
        <source>Vertical offset of image within frame</source>
        <translation>Вертикальне зміщення зображення в рамці</translation>
    </message>
    <message>
        <source>Resize the image horizontally</source>
        <translation>Змінити розмір зображення по горизонталі</translation>
    </message>
    <message>
        <source>Resize the image vertically</source>
        <translation>Змінити розмір зображення по вертикалі</translation>
    </message>
    <message>
        <source>Keep the X and Y scaling the same</source>
        <translation>Утримувати масштаб по вісях X та Y однаковим</translation>
    </message>
    <message>
        <source>Make the image fit within the size of the frame</source>
        <translation>Змусити зображення вміститися в рамку</translation>
    </message>
    <message>
        <source>Use image proportions rather than those of the frame</source>
        <translation>Використовувати відношення сторін зображення, а не рамки</translation>
    </message>
    <message>
        <source>Cell Lines</source>
        <translation>Лінії обрамлення клітин таблиці</translation>
    </message>
    <message>
        <source>Line at Top</source>
        <translation>Лінія верхньої сторони</translation>
    </message>
    <message>
        <source>Line at the Left</source>
        <translation>Лінія лівої сторони</translation>
    </message>
    <message>
        <source>Line at the Right </source>
        <translation>Лінія правої сторони</translation>
    </message>
    <message>
        <source>Line at Bottom</source>
        <translation>Лінія нижньої сторони</translation>
    </message>
    <message>
        <source>Keep the aspect ratio</source>
        <translation>Зберігати відношення сторін</translation>
    </message>
    <message>
        <source>Source profile of the image</source>
        <translation>Стартовий профіль зображення</translation>
    </message>
    <message>
        <source>Rendering intent for the image</source>
        <translation>Схема перерахунку кольорів зображення</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">пік</translation>
    </message>
    <message>
        <source>Switches between Gap or Column width</source>
        <translation>Вибір між шириною стовпців або проміжком між стовпцями</translation>
    </message>
    <message>
        <source>Column width</source>
        <translation>Ширина стовпця</translation>
    </message>
    <message>
        <source>Path Text Properties</source>
        <translation>Властивості текстового шляху</translation>
    </message>
    <message>
        <source>Make text in lower frames flow around the object shape</source>
        <translation>Змусити текст в рамках на задніх планах обтікати форму об&apos;єкта</translation>
    </message>
    <message>
        <source>Indicates the level the object is on, 0 means the object is at the bottom</source>
        <translation>Вказує на план, на якому розташовано об&apos;єкт. Нуль означає, що об&apos;єкт знаходиться на самому нижньому плані</translation>
    </message>
    <message>
        <source>X, Y, &amp;Z</source>
        <translation>X, Y, &amp;Z</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Фігура</translation>
    </message>
    <message>
        <source>&amp;Text</source>
        <translation>&amp;Текст</translation>
    </message>
    <message>
        <source>&amp;Image</source>
        <translation>&amp;Зображення</translation>
    </message>
    <message>
        <source>&amp;Line</source>
        <translation>&amp;Лінія</translation>
    </message>
    <message>
        <source>&amp;Colors</source>
        <translation>&amp;Кольори</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Поз. &amp;Х:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Поз. &amp;У:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висота:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation>&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>&amp;Edit Shape...</source>
        <translation>&amp;Редагувати форму...</translation>
    </message>
    <message>
        <source>R&amp;ound
Corners:</source>
        <translation>З&amp;аокруглення
кутів:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Стовпці:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Проміжок:</translation>
    </message>
    <message>
        <source>To&amp;p:</source>
        <translation>&amp;Верх:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Низ:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лівий край:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Правий край:</translation>
    </message>
    <message>
        <source>T&amp;abulators...</source>
        <translation>&amp;Табулятори...</translation>
    </message>
    <message>
        <source>Text &amp;Flows Around Frame</source>
        <translation>Текст &amp;огинає рамку</translation>
    </message>
    <message>
        <source>Use &amp;Bounding Box</source>
        <translation>Використовувати об&amp;межуючу рамку</translation>
    </message>
    <message>
        <source>&amp;Use Contour Line</source>
        <translation>Використовувати &amp;контурну лінію</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation type="obsolete">&amp;Розмір шрифта:</translation>
    </message>
    <message>
        <source>&amp;Kerning:</source>
        <translation type="obsolete">&amp;Кернінг:</translation>
    </message>
    <message>
        <source>L&amp;ine Spacing:</source>
        <translation type="obsolete">Інтервал мі&amp;ж рядками:</translation>
    </message>
    <message>
        <source>St&amp;yle:</source>
        <translation>&amp;Стиль:</translation>
    </message>
    <message>
        <source>Lan&amp;guage:</source>
        <translation>&amp;Мова:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation>&amp;Вільне масштабування</translation>
    </message>
    <message>
        <source>X-Sc&amp;ale:</source>
        <translation>Масштабування по шкалі &amp;X:</translation>
    </message>
    <message>
        <source>Y-Scal&amp;e:</source>
        <translation>Масштабування по шкалі &amp;Y:</translation>
    </message>
    <message>
        <source>Scale &amp;To Frame Size</source>
        <translation>Установити масштаб по розміру &amp;рамки</translation>
    </message>
    <message>
        <source>P&amp;roportional</source>
        <translation>&amp;Пропорційно</translation>
    </message>
    <message>
        <source>&amp;Basepoint:</source>
        <translation>Точка ві&amp;дліку:</translation>
    </message>
    <message>
        <source>T&amp;ype of Line:</source>
        <translation>&amp;Тип лінії:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation>Тов&amp;щина лінії:</translation>
    </message>
    <message>
        <source>Ed&amp;ges:</source>
        <translation>&amp;Краї:</translation>
    </message>
    <message>
        <source>&amp;Endings:</source>
        <translation>&amp;Кінці:</translation>
    </message>
    <message>
        <source>&amp;X1:</source>
        <translation>&amp;X1:</translation>
    </message>
    <message>
        <source>X&amp;2:</source>
        <translation>&amp;X2:</translation>
    </message>
    <message>
        <source>Y&amp;1:</source>
        <translation>&amp;Y1:</translation>
    </message>
    <message>
        <source>&amp;Y2:</source>
        <translation>&amp;Y2:</translation>
    </message>
    <message>
        <source>Hyphenation language of frame</source>
        <translation>Установки мови переносів для рамки</translation>
    </message>
    <message>
        <source>Use a surrounding box instead of the frame&apos;s shape for text flow</source>
        <translation>Використовувати прямокутник замість форми рамки для огинання тексту</translation>
    </message>
    <message>
        <source>Use a second line originally based on the frame&apos;s shape for text flow</source>
        <translation>Використовувати другу лінію, основану на формі рамки, для огинання тексту</translation>
    </message>
    <message>
        <source>Right to Left Writing</source>
        <translation>Написання зправа наліво</translation>
    </message>
    <message>
        <source>Trac&amp;king:</source>
        <translation type="obsolete">&amp;Слідкування:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Слідкування вручну</translation>
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
</context>
<context>
    <name>MultiLine</name>
    <message>
        <source>Edit Style</source>
        <translation>Редагування стилю</translation>
    </message>
    <message>
        <source>Flat Cap</source>
        <translation>Пласка верхівка</translation>
    </message>
    <message>
        <source>Square Cap</source>
        <translation>Квадратна верхівка</translation>
    </message>
    <message>
        <source>Round Cap</source>
        <translation>Заокруглена верхівка</translation>
    </message>
    <message>
        <source>Miter Join</source>
        <translation>Фацетне з&apos;єднання</translation>
    </message>
    <message>
        <source>Bevel Join</source>
        <translation>Фасочне з&apos;єднання</translation>
    </message>
    <message>
        <source>Round Join</source>
        <translation>Заокруглене з&apos;єднання</translation>
    </message>
    <message>
        <source>Line Width:</source>
        <translation>Товщина лінії:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source> pt </source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>Solid Line</source>
        <translation>Суцільна лінія</translation>
    </message>
    <message>
        <source>Dashed Line</source>
        <translation>Переривчаста лінія</translation>
    </message>
    <message>
        <source>Dotted Line</source>
        <translation>Лінія з точок</translation>
    </message>
    <message>
        <source>Dash Dot Line</source>
        <translation>Переривчасто точкова лінія</translation>
    </message>
    <message>
        <source>Dash Dot Dot Line</source>
        <translation>Риска-точка-точка лінія</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation>Ім&apos;я &quot;%1&quot; вже використане.
Будь-ласка виберіть інше.</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>MusterSeiten</name>
    <message>
        <source>Edit Templates</source>
        <translation type="obsolete">Редагування шаблонів</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Застереження</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation type="obsolete">Копія %1</translation>
    </message>
    <message>
        <source>New Template</source>
        <translation type="obsolete">Новий шаблон</translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="obsolete">Копія №%1 з </translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="obsolete">Нормальний</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="obsolete">Ім&apos;я:</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Добавити</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Новий</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Зачинити</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation type="obsolete">&amp;Ні</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation type="obsolete">&amp;Так</translation>
    </message>
    <message>
        <source>&amp;Name:</source>
        <translation type="obsolete">&amp;Ім&apos;я:</translation>
    </message>
    <message>
        <source>Do you really want to delete this Template?</source>
        <translation type="obsolete">Ви справді бажаєте видалити цей шаблон?</translation>
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
        <source>New Document</source>
        <translation>Новий документ</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Розмір сторінки</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="obsolete">Нестандартний</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Горизонтальна</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Розмітка полів</translation>
    </message>
    <message>
        <source>Options</source>
        <translation>Установки</translation>
    </message>
    <message>
        <source>Points (pts)</source>
        <translation type="obsolete">Точки (тчк)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Дюйми (д)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Піки (п)</translation>
    </message>
    <message>
        <source>Column Guides</source>
        <translation type="obsolete">Розмітка стовпців</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Мілліметри (мм)</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">пік</translation>
    </message>
    <message>
        <source>Document page size, either a standard size or a custom size</source>
        <translation>Розмір сторінки документа - стандартний чи вибраний Вами</translation>
    </message>
    <message>
        <source>Orientation of the document&apos;s pages</source>
        <translation>Орієнтація сторінок документу</translation>
    </message>
    <message>
        <source>Width of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Ширина сторінок документу. Може бути змінена, якщо вибраний нестандартний розмір сторінки</translation>
    </message>
    <message>
        <source>Height of the document&apos;s pages, editable if you have chosen a custom page size</source>
        <translation>Висота сторінок документу. Може бути змінена, якщо вибраний нестандартний розмір сторінки</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Вибрати одиничний чи книжковий перепліт</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Помістити першу сторінку документу зліва</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі верхнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі нижнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі лівого поля сторінки до її краю. Якщо вибраний 
книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі правого поля сторінки до її краю. Якщо вибраний 
книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source>First page number of the document</source>
        <translation>Номер першої сторінки документа</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Стандартна одиниця вимірювання для редагування документа</translation>
    </message>
    <message>
        <source>Create text frames automatically when new pages are added</source>
        <translation>Автоматично створювати текстові рамки на нових сторінках</translation>
    </message>
    <message>
        <source>Distance between automatically created columns</source>
        <translation>Відстань між автоматично створеними стовпцями</translation>
    </message>
    <message>
        <source>Number of columns to create in automatically created text frames</source>
        <translation>Число стовпців в автоматично створених текстових рамках</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Юридичний</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Лист</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Таблоїд</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Розмір:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Орієнтація:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висота:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Суміжні сторінки</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Ліва сторінка перша</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation type="obsolete">&amp;Лівий край:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation type="obsolete">&amp;Правий край:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation type="obsolete">&amp;Верх:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation type="obsolete">&amp;Низ:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Номер &amp;першої сторінки:</translation>
    </message>
    <message>
        <source>&amp;Default Unit:</source>
        <translation>Одиниця виміру по &amp;умовчанню:</translation>
    </message>
    <message>
        <source>&amp;Automatic Text Frames</source>
        <translation>&amp;Автоматичні текстові рамки</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation>&amp;Проміжок:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation>&amp;Стовпці:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Зсередини:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">Зз&amp;овні:</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Виконавчий</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Альбомний</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Бухгалтерський</translation>
    </message>
    <message>
        <source>Open Document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Recent Documents</source>
        <translation type="unfinished"></translation>
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
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation type="unfinished">Всі файли (*)</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="unfinished">Відчинити</translation>
    </message>
</context>
<context>
    <name>NewFromTemplatePlugin</name>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="unfinished">Новий з &amp;шаблона...</translation>
    </message>
</context>
<context>
    <name>NewTm</name>
    <message>
        <source>Left Page</source>
        <translation type="obsolete">Ліва сторінка</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="obsolete">Права сторінка</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>NodePalette</name>
    <message>
        <source>Nodes</source>
        <translation>Вузли</translation>
    </message>
    <message>
        <source>Move Nodes</source>
        <translation>Перемістити вузли</translation>
    </message>
    <message>
        <source>Move Control Points</source>
        <translation>Перемістити контрольні точки</translation>
    </message>
    <message>
        <source>Add Nodes</source>
        <translation>Додати вузли</translation>
    </message>
    <message>
        <source>Delete Nodes</source>
        <translation>Видалити вузли</translation>
    </message>
    <message>
        <source>Reset Control Points</source>
        <translation>Повернути контрольні точки до початкового стану</translation>
    </message>
    <message>
        <source>Reset this Control Point</source>
        <translation>Повернути цю контрольну точку до початкового стану</translation>
    </message>
    <message>
        <source>When checked use Coordinates relative to the Page,
otherwise Coordinates are relative to the Object.</source>
        <translation type="obsolete">Якщо вибрано, використувуються координати відносно до сторінки,
в іншому випадку координати відносні до об&quot;екта.</translation>
    </message>
    <message>
        <source>&amp;Absolute Coordinates</source>
        <translation>&amp;Абсолютні координати</translation>
    </message>
    <message>
        <source>&amp;X-Pos:</source>
        <translation>Поз. &amp;Х:</translation>
    </message>
    <message>
        <source>&amp;Y-Pos:</source>
        <translation>Поз. &amp;У:</translation>
    </message>
    <message>
        <source>Edit &amp;Contour Line</source>
        <translation>Редагувати &amp;контурну лінію</translation>
    </message>
    <message>
        <source>&amp;Reset Contour Line</source>
        <translation>Повернути &amp;контурну лінію в початковий стан</translation>
    </message>
    <message>
        <source>&amp;End Editing</source>
        <translation>&amp;Закінчити редагування</translation>
    </message>
    <message>
        <source>Move Control Points Independently</source>
        <translation>Переміщати контрольні точки незалежно</translation>
    </message>
    <message>
        <source>Move Control Points Symmetrical</source>
        <translation>Переміщати контрольні точки симетрично</translation>
    </message>
    <message>
        <source>Open a Polygon or Cuts a Bezier Curve</source>
        <translation>Роз&apos;єднує полігон або розрізає криву Безьє</translation>
    </message>
    <message>
        <source>Close this Bezier Curve</source>
        <translation>З&apos;єднати кінці кривої Безьє</translation>
    </message>
    <message>
        <source>Mirror the Path Horizontally</source>
        <translation>Віддзеркалити шлях горизонтально</translation>
    </message>
    <message>
        <source>Mirror the Path Vertically</source>
        <translation>Віддзеркалити шлях вертикально</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Left</source>
        <translation>Зсунути шлях горизонтально вліво</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Up</source>
        <translation>Зсунути шлях вертикально вверх</translation>
    </message>
    <message>
        <source>Shear the Path Vertically Down</source>
        <translation>Зсунути шлях вертикально вниз</translation>
    </message>
    <message>
        <source>Rotate the Path Counter-Clockwise</source>
        <translation>Повернути шлях проти годинної стрілки</translation>
    </message>
    <message>
        <source>Rotate the Path Clockwise</source>
        <translation>Повернути шлях по годинній стрілці</translation>
    </message>
    <message>
        <source>Reduce the Size of the Path by shown %</source>
        <translation>Зменшити розмір шляху на вказані %</translation>
    </message>
    <message>
        <source>Enlarge the Size of the Path by shown %</source>
        <translation>Збільшити розмір шляху на вказані %</translation>
    </message>
    <message>
        <source>Angle of Rotation</source>
        <translation>Кут повороту</translation>
    </message>
    <message>
        <source>% to Enlarge or Reduce By</source>
        <translation>% для збільшення або зменшення</translation>
    </message>
    <message>
        <source>Activate Contour Line Editing Mode</source>
        <translation>Активувати режим редагування контурної лінії</translation>
    </message>
    <message>
        <source>Reset the Contour Line to the Original Shape of the Frame</source>
        <translation>Повернути контурну лінію до початкової форми рамки</translation>
    </message>
    <message>
        <source>Shear the Path Horizontally to the Right</source>
        <translation>Зсунути шлях горизонтально вправо</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>When checked use coordinates relative to the page, otherwise coordinates are relative to the Object.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OODrawImportPlugin</name>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="unfinished">Імпортувати файл  &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>Imports OpenOffice Draw Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Imports most OpenOffice Draw files into the current document, converting their vector data into Scribus objects.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OdtDialog</name>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation type="obsolete">Установки імпортера документів OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Поновити стилі абзаців</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Якщо стиль активного абзацу документа OpenOffice.org вже існує, 
стиль в документі Scribus повинет бути відредагованим для 
співпадання з імпортованим стилем чи залишений незмінним</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="obsolete">Упакувати стилі абзаців</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="obsolete">Згрупувати стилі абзаців за атрибутами. Це приводить до
зменшення числа стилів абзаців, але викликає труднощі
для керування ними. Використовуйте, якщо відомо, що 
текст не повинен редагуватися після імпортування.</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Використовувати ім&quot;я документа, як префікс для стилів абзаців</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Чи потрібно, щоб імпортер використовував ім&quot;я
документа як префікс для імен стилів Scribus</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Більше на запитувати</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Чи повинен імпортер завжди використовувати
вибране значення при імпорті документів OpenOffice.org 
і більше не запитувати Вашого підтверждення</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>OpenDocument Importer Options</source>
        <translation>Установки імпортера документів OpenDocument</translation>
    </message>
    <message>
        <source>Update paragraph Styles</source>
        <translation type="obsolete">Поновити стилі абзаців</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Активація цією установки приведе до перезапису існуючих стилів
в активному документі Scribus</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Об&apos;єднати стилі абзаців</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Об&apos;єднати стилі абзаців по атрибутах. Це приведе до зменшення числа схожих
стилів абзаців і збереже атрибути стилів навіть якщо стилі вихідного документу
мали інші імена.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Приєднати назву документа як префікс до імені стилю абзацу в
Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OASIS OpenDocument.</source>
        <translation>Зробити ці установки установками по умовчанню і не запитувати знову під
час імпорту документу OASIS OpenDocument.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OutlineValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
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
        <translation type="obsolete">Консоль сценарія</translation>
    </message>
</context>
<context>
    <name>PDF_Opts</name>
    <message>
        <source>Export Range</source>
        <translation type="obsolete">Діапазон експортування</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="obsolete">Установки файла</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="obsolete">Ліва сторона</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="obsolete">Права сторона</translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="obsolete">тнд</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="obsolete">Загальні</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="obsolete">Вкладання шрифтів</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="obsolete">Доступні шрифти:</translation>
    </message>
    <message>
        <source>Fonts to embed:</source>
        <translation type="obsolete">Шрифти для вкладання:</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Сторінка</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="obsolete">Ефекти</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="obsolete">сек</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="obsolete">Горизонтально</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="obsolete">Вертикально</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="obsolete">Зсередини</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="obsolete">Ззовні</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="obsolete">Паролі</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="obsolete">Установки</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="obsolete">Екран комп&apos;ютера/Веб сторінка</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="obsolete">Принтер</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="obsolete">Однотонні кольори:</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="obsolete">Профіль:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="obsolete">Призначення вихідного зображення:</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="obsolete">Зображення:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="obsolete">Не використовувати внутрішні ICC профілі</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="obsolete">Призначення вихідного документу PDF/X-3</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="obsolete">Обрізати рамку</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="obsolete">Ніякого ефекту</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="obsolete">Венська фіранка</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="obsolete">Коробка</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="obsolete">Розтавання</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="obsolete">Сяяння</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="obsolete">Розділення</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="obsolete">Витирання</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="obsolete">Зліва направо</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="obsolete">Зверху донизу</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="obsolete">Знизу вверх</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="obsolete">Зправа наліво</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="obsolete">Зліва зверху вправо вниз</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="obsolete">Уявний</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="obsolete">Відносна кольорометрія</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="obsolete">Насичення</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="obsolete">Абсолютна кольорометрія</translation>
    </message>
    <message>
        <source>Image Settings</source>
        <translation type="obsolete">Установки зображення</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="obsolete">Автоматичний</translation>
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
        <translation type="obsolete">Ніякого</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="obsolete">Максимальна</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="obsolete">Висока</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="obsolete">Середня</translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="obsolete">Низька</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="obsolete">Мінімальна</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="obsolete">Експортувати всі сторінки в PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="obsolete">Експортувати діапазон сторінок в PDF</translation>
    </message>
    <message>
        <source>Determines the PDF compatibility. The default is Acrobat 4.0 which gives the widest compatibility.
Choose Acrobat 5.0 if your file has PDF 1.4 features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF for commercial printing and is selectable when you have activated color management.</source>
        <translation type="obsolete">Визначає сумісність PDF. Стандартом є Acrobat 4.0, який дає найвищу сумісність.
Виберіть Acrobat 5.0, якщо в документі є риси PDF 1.4, такі як прозорість або 128-бітне шифрування.
PDF/X-3 використовується для експорту PDF для комерційного друку. Він доступний лише при наявності
активованої систему управління кольором.</translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="obsolete">Визначає тип переплітання сторінок в PDF. Стандартний варіант - 
переплітання з лівої сторони.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="obsolete">Вкладає зменшені зображення кожної сторінки в PDF.
Деякі програми для перегляду PDF можуть використовувати
ці зображення для навігації.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="obsolete">Генерувати PDF Статті. Корисні для навігації ссилок на статті в PDF.</translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="obsolete">Вбудувати закладки. які Ви зробили в документі, в PDF.
Корисно для навігації довгих PDF документів.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="obsolete">Розрішення експортованого тексту та зображень.
Не має ніякого ефекту на розрішення растрових зображень,
таких як фотографії.</translation>
    </message>
    <message>
        <source>Compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="obsolete">Компресія тексту та зображень.
Зменшує розмір PDF. Не змінюйте без явної причини.</translation>
    </message>
    <message>
        <source>Version of compression for images.
Automatic allows Scribus to choose the best method.
ZIP is good for images with solid colors.
JPEG is better at creating smaller PDF files which have many photos (with slight image loss possible).
Leave it set to automatic, unless you have a need for special compression options.</source>
        <translation type="obsolete">Версія компресії для зображень.
Автоматична - дозволяє Scribus вибрати найкращий метод.
ZIP - добре використовувати для зображень з однотонними кольорами.
JPEG - добре використовувати при створенні невеликих PDF файлів в які
вставлено багато фотографічних зображень. Можлива невелика втрата
якості зображення.
Залиште установку на автоматичну компресію при відсутності явної причини для
використання більш специфічних схем компресії.</translation>
    </message>
    <message>
        <source>Downsample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.</source>
        <translation type="obsolete">Перерахувати розрішення растрових зображень до вибраних точок/кв. дюйм (DPI).
Якщо ця опція не установлена то буде використане вихідне розрішення зображень.</translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="obsolete">DPI (Точок на квадратний дюйм) для експорту зображень.</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="obsolete">Вбудувати шрифти в PDF. Вбудовування шрифтів.
збереже формат і вигляд документу.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Acrobat Reader in full screen mode.</source>
        <translation type="obsolete">Дозволяє використовувати презентаційні ефекти при використанні Acrobat
Reader в повноекранному режимі.</translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="obsolete">Показувати попередній перегляд кожної сторінки вказаної вверху.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="obsolete">Тривалість показу сторінки перед початком прозентації на вибраній сторінці.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="obsolete">Тривалість ефекту. Менша тривалість пришвидшуе ефект і навпаки.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="obsolete">Тип ефекту.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="obsolete">Напрямок руху ліній для &quot;розділений&quot; і &quot;венеціанська штора&quot; ефектів.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="obsolete">Початкова позиція для ефектів &quot;коробка&quot; і &quot;розділений&quot;.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="obsolete">Напрямок руху ефектів &quot;блиск&quot; і &quot;витирання&quot;.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="obsolete">Застосувати вибраний ефект на всіх сторінках.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected Acrobat 4.0, the PDF will be protected by 40 bit encryption.
If you selected Acrobat 5.0, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="obsolete">Ввімкнути засоби захисту в експортованому PDF.
Якщо Ви вибрали Acrobat 4.0, то PDF буде захищено 40-бітним шифруванням.
Якщо Ви вибрали Acrobat 5.0, то PDF буде захищено 128-бітним шифруванням.
Застереження: Шифрування PDF не має такої надійності, як GPG або PGP шифрування і вносить додаткові обмеження.</translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="obsolete">Виберіть головний пароль для ввімкнення та вимкнення всіх
засобів захисту в експортованому PDF</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="obsolete">Кольорова модель для створення PDF.
Виберіть Екран/Веб для PDF документів, призначених для показу на екрані або для друку на типових
струменевих прінтерах.
Виберіть Прінтер для друку на справжніх 4-х кольорових CMYK прінтерах.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="obsolete">Вбудувати кольоровий профіль для однотонних кольорів</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="obsolete">Кольоровий профіль для однотонних кольорів</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="obsolete">Схема перерахунку кольорів для однотонних кольорів</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="obsolete">Вбудований кольоровий профіль для зображень</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="obsolete">Не використовувати кольорові профілі вбудовані в оригінальні зображення</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="obsolete">Кольоровий профіль для зображень</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="obsolete">Схема перерахунку кольорів для зображень</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="obsolete">Профіль для друку. При можливості скористайтеся порадами 
технолога типографії для правильного вибору цього профіля.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="obsolete">Цей запис вимагається форматом PDF/X-3. Без нього PDF не пройде перевірку
на відповідність формату PDF/X-3. Ми рекомендуємо використання заголовку документу.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="obsolete">Розмір поля для перекриття з верхньої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="obsolete">Розмір поля для перекриття з нижньої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="obsolete">Розмір поля для перекриття з лівої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="obsolete">Розмір поля для перекриття з правої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="obsolete">&amp;Загальні</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="obsolete">&amp;Шрифти</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">пік</translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="obsolete">Ступені стиснення:  Мінімальна (25%), Низка (50%), Середня (75%), Висока (85%), Максимальна (95%)</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="obsolete">Виберіть пароль для захисту PDF документа від перегляду.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="obsolete">Дозволити друк PDF. Якщо не вибрати цей параметр, то друк буде заборонено.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="obsolete">Дозволити редагування PDF. Якщо цей параметр не вибрати, то редагування PDF буде заборонено.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="obsolete">Дозволити копіювання тексту і зображень з PDF.
Якщо цей параметр не вибрати, то копіювання 
тексту і зображень буде заборонено.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="obsolete">Дозволити вставку анотацій та полів в PDF.
Якщо цей параметр не вибрати, то редагування
анотацій та полів буде заборонено.</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="obsolete">Вмістити список елементів, розділений комами де елемент
може *бути для всіх сторінок, 1-5 для діапазона сторінок, або
номер окремої сторінки.</translation>
    </message>
    <message>
        <source>Create PDF File</source>
        <translation type="obsolete">Створити PDF файл</translation>
    </message>
    <message>
        <source>O&amp;utput to File:</source>
        <translation>За&amp;писати в файл:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="obsolete">&amp;Всі сторінки</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="obsolete">&amp;Виберіть сторінки</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="obsolete">&amp;Сумісність:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="obsolete">&amp;Перепліт:</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="obsolete">Створити &amp;Мініатюрні зображення</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="obsolete">Зберегти &amp;зв&apos;язані текстові рамки, як статті PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="obsolete">Включити &amp;закладки</translation>
    </message>
    <message>
        <source>&amp;Resolution:</source>
        <translation type="obsolete">&amp;Розрішення:</translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="obsolete">&amp;Метод:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="obsolete">&amp;Якість:</translation>
    </message>
    <message>
        <source>&amp;Downsample Images to:</source>
        <translation type="obsolete">Зменшити &amp;розрішення зображень до:</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="obsolete">&amp;Вкласти всі шрифти</translation>
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
        <source>Show Page Pre&amp;views</source>
        <translation type="obsolete">&amp;Попередній перегляд сторінок</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="obsolete">&amp;Тривалість показу:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="obsolete">Тривалість &amp;ефекту:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="obsolete">Т&amp;ип ефекту:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="obsolete">&amp;Рухливі лінії:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="obsolete">&amp;Від:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="obsolete">&amp;Напрямок:</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="obsolete">Застосувати ефект на в&amp;сіх сторінках</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="obsolete">Використовувати &amp;шифрування</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="obsolete">&amp;Користувач:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="obsolete">В&amp;ласник:</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="obsolete">Дозволити &amp;друк документа</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="obsolete">Дозволити внесення &amp;змін в документ</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="obsolete">Дозволити &amp;копіювання тексту і зображень</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="obsolete">Дозволити додавання &amp;аннотацій та полів</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="obsolete">&amp;Захист</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="obsolete">Вихідний документ &amp;призначений для:</translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="obsolete">Використовувати нестандартні &amp;установки рендерингу</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="obsolete">Установки рендерингу</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="obsolete">&amp;Частота:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="obsolete">&amp;Кут:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="obsolete">&amp;Локальна функція:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="obsolete">Проста точка</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Лінія</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="obsolete">Круг</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="obsolete">Елліпс</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="obsolete">Використовувати ICC профіль</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="obsolete">&amp;Колір</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="obsolete">&amp;Інформація:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="obsolete">Профіль &amp;виводу:</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="obsolete">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Записати</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>PDF Files (*.pdf);;All Files (*)</source>
        <translation>PDF файли (*.pdf);;Всі файли (*)</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="obsolete">Це додаткова установка, яка не використовується по умовчанню. Вона має бути ввімкнена
лише по вимозі типографії і наявності точних деталей. Інакше експортований PDF може не
друкуватися правильно і не бути придатним для використання на різних платформах.</translation>
    </message>
    <message>
        <source>Compress Text and &amp;Vector Graphics</source>
        <translation type="obsolete">Стиснути текст та &amp;векторну графіку</translation>
    </message>
    <message>
        <source>En&amp;able Presentation Effects</source>
        <translation type="obsolete">Вв&amp;імкнути презентаційні ефекти</translation>
    </message>
    <message>
        <source>&amp;Presentation</source>
        <translation type="obsolete">&amp;Презентація</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="obsolete">Провести вибірку всіх &amp;шрифтів</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="obsolete">Шрифти для вибірки:</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="obsolete">Віддзеркалити сторінку(и) горизонтально</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="obsolete">Віддзеркалити сторінку(и) вертикально</translation>
    </message>
    <message>
        <source>Save as PDF</source>
        <translation>Записати як PDF</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management must be enabled to use PDF/X-3. You can enable color management from the Settings menu.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Керування кольорами повинне бути активоване для використання PDF/X-3. Ви можете активувати керування кольорами в меню Установки.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is supported and enabled, but can not be used for the selected PDF version. If you want to use PDF/X-3, you need to set PDF/X-3 as your PDF version (compatibility level).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;PDF/X-3 підтримується і активний, але не може бути використаним в вибраній версії PDF. Якщо Ви бажаєте використати PDF/X-3, Вам потрібно установити PDF/X-3, як версію PDF (рівень сумісності).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF/X-3 is not supported by this Scribus build (CMS support not present).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;PDF/X-3 не підтримується цією збіркою Scribus (CMS підтримка не була вбудована при збірці програми з сирців).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fields is prevented.</source>
        <translation type="obsolete">Дозволити додавання аннотацій та полів в PDF.
Якщо не ввімкнене, редагування аннотацій та полів
не дозволяється.</translation>
    </message>
    <message>
        <source>PDF security settings</source>
        <translation type="obsolete">Установки безпеки PDF</translation>
    </message>
    <message>
        <source>PDF/X-3 settings</source>
        <translation type="obsolete">Установки PDF/X-3</translation>
    </message>
    <message>
        <source>&lt;qt&gt;PDF security can not be used with PDF/X-3. If you want to turn on security, change your PDF version (compatibility level) to something other than PDF/X-3.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Засоби безпеки PDF не можуть використовуватися в PDF/X-3. Якщо ви бажаєте ввімкнути засоби безпеки PDF, змініть версію (рівень сумісності) на якусь іншу крім PDF/X-3.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Output one file for eac&amp;h page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This enables exporting one individually named PDF file for each page in the document. Page numbers are added automatically. This is most useful for imposing PDF for commerical printing.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PPreview</name>
    <message>
        <source>Print Preview</source>
        <translation>Перегляд перед друком</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of text items in the viewer, at the expense
of a slight slowdown in previewing. This only affects Type 1 fonts</source>
        <translation type="obsolete">Покращує вигляд текстових об&apos;єктів при перегляді за рахунок деякого
сповільнення процесу. Впливає лише на шрифти Type 1</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of True Type Fonts, Open Type Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Покращує вигляд True Type шрифтів, Open Type шрифтів, EPS, PDF та векторної
графіки при перегляді за рахунок деякого сповільнення процесу перегляду</translation>
    </message>
    <message>
        <source>Shows transparency and transparent items in your document. Requires Ghostscript 7.07 or later</source>
        <translation>Показує прозорість та прозорі об&apos;єкти в документі. Вимагає Ghostscript 7.07 чи новіший</translation>
    </message>
    <message>
        <source>Gives a print preview using simulations of generic CMYK inks, instead of RGB colors</source>
        <translation>Створює перегляд перед друком використовуючи симуляцію  
звичайних  CMYK чорнил замість RGB кольорів</translation>
    </message>
    <message>
        <source>Enable/disable the C (Cyan) ink plate</source>
        <translation>Ввімкнути/вимкнути C (Салатову) чорнильну пластину</translation>
    </message>
    <message>
        <source>Enable/disable the M (Magenta) ink plate</source>
        <translation>Ввімкнути/вимкнути М (Малинову) чорнильну пластину</translation>
    </message>
    <message>
        <source>Enable/disable the Y (Yellow) ink plate</source>
        <translation>Ввімкнути/вимкнути Y (Жовту) чорнильну пластину</translation>
    </message>
    <message>
        <source>Enable/disable the K (Black) ink plate</source>
        <translation>Ввімкнути/вимкнути К (Чорну) чорнильну пластину</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Text</source>
        <translation>Антиаліасинг &amp;тексту</translation>
    </message>
    <message>
        <source>Anti-alias &amp;Graphics</source>
        <translation>Антиаліасинг &amp;графіки</translation>
    </message>
    <message>
        <source>Display Trans&amp;parency</source>
        <translation>Показувати п&amp;розорість</translation>
    </message>
    <message>
        <source>&amp;Display CMYK</source>
        <translation>Показувати &amp;CMYK</translation>
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
        <translation>У &amp;видаленні &amp;кольорів</translation>
    </message>
    <message>
        <source>A way of switching some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Метод переводу певних відтінків сірого, які складаються з салатового, 
жовтого та рожевого кольорів, на використання чорного кольору.
UCR найчастіше впливає на ті частини зображень, які складаються з нейтральних
та/чи темних відтінків близьких до сірого. Використання цієї установки може
покращити якість друку деяких зображень. Деяке тестування та експерименти
необхідні в кожному конкретному випадку. UCR знижує ймовірність перенасичення
паперу CMY чорнилами.</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation type="obsolete">Метод переходу від деяких відтінків сірого, які складаються
з салатного, жовтого та малинового кольорів до використання
чорного кольору.
UCR найбільше впливає на частини зображень з нейтральними
та темними тонами, близькими до сірого кольору. Його використання
може покращити друк деяких зображень. Деяке експериментування
та тестування може бути необхідним для досягнення потрібного
результату. UCR знижує можливість перенасичення паперу CMY 
чорнилами.</translation>
    </message>
    <message>
        <source>Provides a more pleasant view of TrueType Fonts, OpenType Fonts, EPS, PDF and
vector graphics in the preview, at the expense of a slight slowdown in previewing</source>
        <translation type="obsolete">Надає кращий вигляд шрифтам TrueType, OpenType, EPS, PDF та векторній графіці
в попередньому перегляді за рахунок невеликого сповільнення процесу перегляду</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Застереження</translation>
    </message>
    <message>
        <source>Separation Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Салатовий</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Малиновий</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Жовтий</translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Чорний</translation>
    </message>
    <message>
        <source>Scaling:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="unfinished">Друк...</translation>
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
</context>
<context>
    <name>Page</name>
    <message>
        <source>Picture</source>
        <translation type="obsolete">Зображення</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="obsolete">Файл:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="obsolete">Зв&apos;язаний текст</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="obsolete">Текстова рамка</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="obsolete">Текст на шляху</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="obsolete">Абзаци:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="obsolete">Слова:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="obsolete">Символи:</translation>
    </message>
    <message>
        <source>Edit Text...</source>
        <translation type="obsolete">Редагувати текст...</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Немає</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="obsolete">Друкувати:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="obsolete">Ввімкнено</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="obsolete">Вимкнено</translation>
    </message>
    <message>
        <source>The Program</source>
        <translation type="obsolete">Програма</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">не існує!</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Застереження</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Копія</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="obsolete">Копіювати сюди</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="obsolete">Перемістити сюди</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Вклеїти</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Показати &amp;розмітку полів</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Показати ра&amp;мки</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">&amp;Показати зображення</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Показати &amp;сітку</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Показати розм&amp;ітку</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Показати &amp;базову сітку</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">П&amp;ритягування до сітки</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Притягування до розмітк&amp;и</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="obsolete">Початкові ТНД: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="obsolete">Фактичні ТНД:</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="obsolete">&amp;Інформація</translation>
    </message>
    <message>
        <source>&amp;Get Picture...</source>
        <translation type="obsolete">Вставити &amp;зображення...</translation>
    </message>
    <message>
        <source>I&amp;mage Visible</source>
        <translation type="obsolete">&amp;Показувати зображення</translation>
    </message>
    <message>
        <source>&amp;Update Picture</source>
        <translation type="obsolete">Поновити &amp;зображення</translation>
    </message>
    <message>
        <source>&amp;Edit Picture</source>
        <translation type="obsolete">&amp;Редагувати зображення</translation>
    </message>
    <message>
        <source>&amp;Adjust Frame to Picture</source>
        <translation type="obsolete">Установити розмір рамки по &amp;зображенню</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Вставити &amp;текст...</translation>
    </message>
    <message>
        <source>&amp;Append Text...</source>
        <translation type="obsolete">Додати &amp;текст...</translation>
    </message>
    <message>
        <source>&amp;Edit Text...</source>
        <translation type="obsolete">&amp;Редагувати текст...</translation>
    </message>
    <message>
        <source>&amp;Insert Sample Text</source>
        <translation type="obsolete">Вставити &amp;пробний текст</translation>
    </message>
    <message>
        <source>Is PDF &amp;Bookmark</source>
        <translation type="obsolete">&amp;Закладка PDF</translation>
    </message>
    <message>
        <source>Is PDF A&amp;nnotation</source>
        <translation type="obsolete">&amp;Аннотація PDF</translation>
    </message>
    <message>
        <source>Annotation P&amp;roperties</source>
        <translation type="obsolete">В&amp;ластивості аннотації</translation>
    </message>
    <message>
        <source>Field P&amp;roperties</source>
        <translation type="obsolete">Властивості &amp;поля</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="obsolete">&amp;Установки PDF</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Зам&amp;кнути</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Відімкнути</translation>
    </message>
    <message>
        <source>Lock Object &amp;Size</source>
        <translation type="obsolete">Замкнути &amp;розмір об&apos;єкта</translation>
    </message>
    <message>
        <source>Unlock Object &amp;Size</source>
        <translation type="obsolete">Розімкнути &amp;розмір об&apos;єкта</translation>
    </message>
    <message>
        <source>Send to S&amp;crapbook</source>
        <translation type="obsolete">Послат в &amp;чорновик</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="obsolete">Перемістити на &amp;план</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">З&amp;групувати</translation>
    </message>
    <message>
        <source>Un&amp;group</source>
        <translation type="obsolete">&amp;Розгрупувати</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="obsolete">Пла&amp;н</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Опустити на &amp;нижній рівень</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Підняти на &amp;верхній рівень</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Опустити</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Підняти</translation>
    </message>
    <message>
        <source>&amp;Picture Frame</source>
        <translation type="obsolete">Рамка &amp;зображення</translation>
    </message>
    <message>
        <source>Pol&amp;ygon</source>
        <translation type="obsolete">Полі&amp;гон</translation>
    </message>
    <message>
        <source>&amp;Outlines</source>
        <translation type="obsolete">&amp;Контури</translation>
    </message>
    <message>
        <source>&amp;Text Frame</source>
        <translation type="obsolete">&amp;Текстова рамка</translation>
    </message>
    <message>
        <source>&amp;Bezier Curve</source>
        <translation type="obsolete">Крива &amp;Безьє</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="obsolete">&amp;Конвертувати в</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Ви&amp;різати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Видалити</translation>
    </message>
    <message>
        <source>C&amp;lear Contents</source>
        <translation type="obsolete">Очистити &amp;вміст</translation>
    </message>
    <message>
        <source>Show P&amp;roperties...</source>
        <translation type="obsolete">Показати &amp;властивості...</translation>
    </message>
    <message>
        <source>Hide P&amp;roperties...</source>
        <translation type="obsolete">Сховати &amp;властивості...</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ви справді бажаєте очистити весь Ваш текст?</translation>
    </message>
</context>
<context>
    <name>PageItem</name>
    <message>
        <source>Image</source>
        <translation>Зображення</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Line</source>
        <translation>Лінія</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation>Полігон</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation>Багатосегментна лінія</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation>Текст на шляху</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="unfinished">Копія</translation>
    </message>
</context>
<context>
    <name>PageItemAttributes</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <source>Page Item Attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name</source>
        <translation type="unfinished">Ім&apos;я</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="unfinished">Тип</translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">Значення</translation>
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
        <translation type="unfinished">&amp;Додати</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="unfinished">&amp;Скопіювати</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="unfinished">Alt+C</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="unfinished">О&amp;чистити</translation>
    </message>
    <message>
        <source>Alt+L</source>
        <translation type="unfinished">Alt+L</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="unfinished">В&amp;ихід</translation>
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
    <name>PageSelector</name>
    <message>
        <source>Page </source>
        <translation type="obsolete">Сторінка</translation>
    </message>
    <message>
        <source> of %1</source>
        <translation type="obsolete"> з %1</translation>
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
        <translation type="unfinished">Лист</translation>
    </message>
    <message>
        <source>Government Letter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="unfinished">Юридичний</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="unfinished">Бухгалтерський</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="unfinished">Виконавчий</translation>
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
        <translation type="unfinished">С</translation>
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
        <translation>Результат</translation>
    </message>
    <message>
        <source>Search Results for: </source>
        <translation>Результати пошуку для:</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Попередній перегляд</translation>
    </message>
    <message>
        <source>Select</source>
        <translation>Вибрати</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
</context>
<context>
    <name>PicStatus</name>
    <message>
        <source>Pictures</source>
        <translation type="obsolete">Зображення</translation>
    </message>
    <message>
        <source>Goto</source>
        <translation>Перейти до</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Так</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Missing</source>
        <translation>Відсутні(й)</translation>
    </message>
    <message>
        <source>Search</source>
        <translation>Пошук</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ім&apos;я</translation>
    </message>
    <message>
        <source>Path</source>
        <translation>Шлях</translation>
    </message>
    <message>
        <source>Page</source>
        <translation>Сторінка</translation>
    </message>
    <message>
        <source>Print</source>
        <translation>Друк</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Статус</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Cancel Search</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="unfinished">Керування зображеннями</translation>
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
</context>
<context>
    <name>PixmapExportPlugin</name>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="unfinished">Записати, як зо&amp;браження...</translation>
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
        <translation type="obsolete">Імпортувати</translation>
    </message>
    <message>
        <source>Unknown</source>
        <comment>plugin manager</comment>
        <translation type="obsolete">Невідомий</translation>
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
    <message>
        <source>PostScript Files (*.eps *.EPS *.ps *.PS);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg *.svgz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>SVG Images (*.svg);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;</source>
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
        <translation type="unfinished">Тип</translation>
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
        <translation type="unfinished">Файл</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="unfinished">Так</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="unfinished">Ні</translation>
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
        <translation>Властивості полігонів</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">К&amp;ути:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Фактор:</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Число кутів полігонів</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Кут повороту полігонів</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Зразок полігону</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Застосувати &amp;фактор</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Застосувати випуклий/ввігнутий фактор для зміни форми полігонів</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Негативне число зробить полігон ввігнутим (або зіркоподібним),
позитивне число зробить його випуклим</translation>
    </message>
</context>
<context>
    <name>PolygonWidget</name>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="unfinished">К&amp;ути:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="unfinished">Застосувати &amp;фактор</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="unfinished">&amp;Фактор:</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="unfinished">Число кутів полігонів</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="unfinished">Кут повороту полігонів</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="unfinished">Застосувати випуклий/ввігнутий фактор для зміни форми полігонів</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="unfinished">Зразок полігону</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="unfinished">Негативне число зробить полігон ввігнутим (або зіркоподібним),
позитивне число зробить його випуклим</translation>
    </message>
</context>
<context>
    <name>Preferences</name>
    <message>
        <source>Preferences</source>
        <translation>Установки</translation>
    </message>
    <message>
        <source>General</source>
        <translation>Загальні</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation>Розмітка</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation>Типографія</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation>Інструменти</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation>Чорновик</translation>
    </message>
    <message>
        <source>Display</source>
        <translation>Відображення</translation>
    </message>
    <message>
        <source>GUI</source>
        <translation>Графічна оболонка</translation>
    </message>
    <message>
        <source>Units</source>
        <translation type="obsolete">Одиниці</translation>
    </message>
    <message>
        <source>Points (pt)</source>
        <translation type="obsolete">Точки (тчк)</translation>
    </message>
    <message>
        <source>Millimetres (mm)</source>
        <translation type="obsolete">Мілліметри (мм)</translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="obsolete">Дюйми (д)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="obsolete">Піки (п)</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation type="obsolete">Меню</translation>
    </message>
    <message>
        <source>Paths</source>
        <translation>Шляхи</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Розмір сторінки</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Нестандартний</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Горизонтальна</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Розмітка полів</translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation>Автоматичне збереження</translation>
    </message>
    <message>
        <source>min</source>
        <translation>хв</translation>
    </message>
    <message>
        <source>Grid Layout</source>
        <translation type="obsolete">Розташування сітки</translation>
    </message>
    <message>
        <source>Grid Colors</source>
        <translation type="obsolete">Кольори сітки</translation>
    </message>
    <message>
        <source>Placing</source>
        <translation type="obsolete">Розміщення</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Нижній індекс</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="obsolete">%</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Верхній індекс</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Капітель</translation>
    </message>
    <message>
        <source>Other</source>
        <translation type="obsolete">Інше</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="obsolete">Реве та стогне Дніпр широкий, сердитий вітер завива</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="obsolete">Немає</translation>
    </message>
    <message>
        <source>Other Options</source>
        <translation>Інші установки</translation>
    </message>
    <message>
        <source>Preview</source>
        <translation>Попередній перегляд</translation>
    </message>
    <message>
        <source>Small</source>
        <translation>Малий</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation>Середній</translation>
    </message>
    <message>
        <source>To adjust the display drag the ruler below with the Slider.</source>
        <translation type="obsolete">Щоб відрегулювати відображення, перемістіть повзунок внизу.</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation>Вибрати директорію</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>External Tools</source>
        <translation>Зовнішні інструменти</translation>
    </message>
    <message>
        <source>Postscript Interpreter</source>
        <translation type="obsolete">Інтерпретатор постскрипта</translation>
    </message>
    <message>
        <source>Image Processing Tool</source>
        <translation>Інструмент для обробки зображень</translation>
    </message>
    <message>
        <source>Misc.</source>
        <translation type="obsolete">Різне.</translation>
    </message>
    <message>
        <source>Printing</source>
        <translation>Друк</translation>
    </message>
    <message>
        <source>Choose the default window decoration and looks.
Scribus inherits any available KDE or Qt themes</source>
        <translation>Виберіть стандартне оздоблення вікна та вигляд.
Scribus успадкує любі доступні теми KDE або Qt</translation>
    </message>
    <message>
        <source>Default font size for the menus and windows</source>
        <translation>Стандартний розмір шрифта для меню та вікон</translation>
    </message>
    <message>
        <source>Default unit of measurement for document editing</source>
        <translation>Стандартна одиниця вимірювання для редагування документу</translation>
    </message>
    <message>
        <source>Number of lines Scribus will scroll for each move of the mouse wheel</source>
        <translation>Число рядків для прокручування на кожний рух колеса мишки</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="obsolete">Радіус площі захвату вузлів об&apos;єктів</translation>
    </message>
    <message>
        <source>Number of recently edited documents to show in the File menu</source>
        <translation>Скільки недавно редагованих документів показувати в меню Файл</translation>
    </message>
    <message>
        <source>Default documents directory</source>
        <translation>Стандартна директорія для документів</translation>
    </message>
    <message>
        <source>Default ICC profiles directory</source>
        <translation type="obsolete">Стандартна директорія для ICC профілів</translation>
    </message>
    <message>
        <source>Default Scripter scripts directory</source>
        <translation>Стандартна директорія для сценаріїв Сценариста</translation>
    </message>
    <message>
        <source>Default page size, either a standard size or a custom size</source>
        <translation>Стандартний розмір сторінки</translation>
    </message>
    <message>
        <source>Default orientation of document pages</source>
        <translation>Стандартна орієнтація сторінок документа</translation>
    </message>
    <message>
        <source>Width of document pages, editable if you have chosen a custom page size</source>
        <translation>Ширина сторінок документа. Її можна змінити, якщо Ви вибрали нестандартний розмір сторінки</translation>
    </message>
    <message>
        <source>Height of document pages, editable if you have chosen a custom page size</source>
        <translation>Висота сторінок документа. Її можна змінити, якщо Ви вибрали нестандартний розмір сторінки</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Вибрати одиничний чи книжковий перепліт</translation>
    </message>
    <message>
        <source>Make the first page the left page of a document</source>
        <translation type="obsolete">Зробити першу сторінку лівою сторінкою</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі верхнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі нижнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі лівого поля сторінки до її краю.
Якщо вибраний книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі правого поля сторінки до її краю.
Якщо вибраний книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source>When enabled, Scribus saves a backup copy of your file with the .bak extension
each time the time period elapses</source>
        <translation>Якщо ввімкнути цю установку, то Scribus записуватиме резервну 
копію файла з розширенням .bak після визначеного часу</translation>
    </message>
    <message>
        <source>Time period between saving automatically</source>
        <translation>Період часу між двома автоматичними записами файла</translation>
    </message>
    <message>
        <source>Distance between the minor grid lines</source>
        <translation type="obsolete">Відстань між лініями допоміжної сітки</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="obsolete">Відстань між лініями головної сітки</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="obsolete">Зона притягування об&apos;єкта до розмітки</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="obsolete">Колір ліній допоміжної сітки</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="obsolete">Колір ліній головної сітки</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="obsolete">Колір ліній розмітки</translation>
    </message>
    <message>
        <source>Place the grid behind your page objects</source>
        <translation type="obsolete">Розмістити сітку під об&apos;єктами на сторінці</translation>
    </message>
    <message>
        <source>Place the grid in front of your page objects</source>
        <translation type="obsolete">Розмістити сітку над об&apos;єктами на сторінці</translation>
    </message>
    <message>
        <source>Displacement above the baseline of the font on a line</source>
        <translation type="obsolete">Зміщення над основою шрифта на лінії</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="obsolete">Відносний розмір верхнього індексу у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="obsolete">Зміщення під основою шрифта на лінії</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="obsolete">Відносний розмір нижнього індексу у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="obsolete">Відносний розмір капітелі у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="obsolete">Збільшення інтервалу між рядками у відсотках розміру шрифта</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="obsolete">Властивості текстової рамки</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="obsolete">Властивості рамки зображення</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="obsolete">Властивості рисування геометричних фігур</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="obsolete">Стандартні установки масштабу зображення</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="obsolete">Властивості рисування лінії</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="obsolete">Властивості рисування полігонів</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="obsolete">Шрифт для нових текстових рамок</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="obsolete">Розмір шрифта для нових текстових рамок</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="obsolete">Колір шрифта</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="obsolete">Число стовпців в текстовій рамці</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="obsolete">Проміжок між стовпцями текстової рамки</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="obsolete">Зразок Вашого шрифта</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="obsolete">Рамки для зображень дозволяють масштабування зображень до любого розміру</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="obsolete">Горизонтальне масштабування зображень</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="obsolete">Вертикальне масштабування зображень</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="obsolete">Утримувати горизонтальне масштабування рівним вертикальному</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="obsolete">Зображення у рамках для зображень масштабовані до розміру рамки</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="obsolete">Автоматично масштабовані зображення зберігають відношення сторін</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="obsolete">Колір заповнення рамок зображень</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="obsolete">Насиченість кольору заповнення</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="obsolete">Колір ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="obsolete">Насиченість кольору ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="obsolete">Колір заповнення геометричних фігур</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="obsolete">Стиль ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="obsolete">Ширина ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="obsolete">Мінімально дозволений масштаб зображення</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="obsolete">Максимально дозволений масштаб зображення</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="obsolete">Зміна масштабу зображення при кожному кроці масштабування</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="obsolete">Колір ліній</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="obsolete">Насиченість кольору</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="obsolete">Стиль ліній</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="obsolete">Товщина ліній</translation>
    </message>
    <message>
        <source>Number of corners for polygons</source>
        <translation type="obsolete">Число кутів полігонів</translation>
    </message>
    <message>
        <source>Degrees of rotation for polygons</source>
        <translation type="obsolete">Кут повороту полігонів</translation>
    </message>
    <message>
        <source>Sample Polygon</source>
        <translation type="obsolete">Зразок полігону</translation>
    </message>
    <message>
        <source>Choose the size of the preview in the scrapbook palette</source>
        <translation>Змінити розмір попереднього перегляду в палітрі чорновика</translation>
    </message>
    <message>
        <source>When using facing pages, show the two pages side by side</source>
        <translation type="obsolete">При використанні книжкового перепліту показувати обидві сторінки поруч</translation>
    </message>
    <message>
        <source>Color for paper</source>
        <translation>Колір паперу</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="obsolete">Колір границь полів сторінки</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation>Замаскувати область за полями кольором полів</translation>
    </message>
    <message>
        <source>Enable transparency features within PDF 1.4 export</source>
        <translation type="obsolete">Ввімкнути риси прозорісті при експорті PDF 1.4</translation>
    </message>
    <message>
        <source>Set the default zoom level</source>
        <translation>Установити стандартний масштаб зображення</translation>
    </message>
    <message>
        <source>Filesystem location for the Ghostscript interpreter</source>
        <translation type="unfinished">Розміщення інтерпретатора Ghostscript на жорсткому диску</translation>
    </message>
    <message>
        <source>Antialias text for EPS and PDF onscreen rendering</source>
        <translation>Антиаліасинг тексту для рендерингу EPS та PDF на екрані</translation>
    </message>
    <message>
        <source>Antialias graphics for EPS and PDF onscreen rendering</source>
        <translation>Антиаліасинг графіки для рендерингу EPS та PDF на екрані</translation>
    </message>
    <message>
        <source>Do not show objects outside the margins on the printed page or exported file</source>
        <translation>Не показувати об&apos;єкти за полями на друкованій сторінці чи в експортованому файлі</translation>
    </message>
    <message>
        <source>Save the scrapbook contents everytime after a change</source>
        <translation>Зберегти вміст чорновика після зміни</translation>
    </message>
    <message>
        <source>Filesystem location for graphics editor</source>
        <translation type="obsolete">Місцезнаходження графічного редактора на жорсткому диску</translation>
    </message>
    <message>
        <source>Baseline Grid</source>
        <translation type="obsolete">Базова сітка</translation>
    </message>
    <message>
        <source>Turns on the basegrid</source>
        <translation type="obsolete">Ввімкнути базову сітку</translation>
    </message>
    <message>
        <source>Turns off the basegrid</source>
        <translation type="obsolete">Вимкнути базову сітку</translation>
    </message>
    <message>
        <source> px</source>
        <translation type="obsolete">пікс</translation>
    </message>
    <message>
        <source>&amp;Theme:</source>
        <translation>&amp;Тема:</translation>
    </message>
    <message>
        <source>&amp;Font Size:</source>
        <translation>&amp;Розмір шрифта:</translation>
    </message>
    <message>
        <source>Mouse Settings</source>
        <translation type="obsolete">Установки миші</translation>
    </message>
    <message>
        <source>&amp;Wheel Jump:</source>
        <translation>&amp;Крок колесика миші:</translation>
    </message>
    <message>
        <source>&amp;Grab Radius:</source>
        <translation type="obsolete">&amp;Радіус захоплення:</translation>
    </message>
    <message>
        <source>&amp;Recent Documents:</source>
        <translation>&amp;Недавно редаговані документи:</translation>
    </message>
    <message>
        <source>&amp;Documents:</source>
        <translation>Док&amp;ументи:</translation>
    </message>
    <message>
        <source>&amp;Change...</source>
        <translation>&amp;Замінити...</translation>
    </message>
    <message>
        <source>&amp;ICC Profiles:</source>
        <translation>&amp;ICC профілі:</translation>
    </message>
    <message>
        <source>C&amp;hange...</source>
        <translation>&amp;Замінити...</translation>
    </message>
    <message>
        <source>&amp;Scripts:</source>
        <translation>&amp;Сценарії:</translation>
    </message>
    <message>
        <source>Ch&amp;ange...</source>
        <translation>&amp;Змінити...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Розмір:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation>&amp;Орієнтація:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation>&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation>&amp;Висота:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Суміжні сторінки</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Ліва сторінка перша</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Низ:</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Верх:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Правий край:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лівий край:</translation>
    </message>
    <message>
        <source>&amp;Enabled</source>
        <translation>Вв&amp;імкнено</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation>&amp;Інтервал:</translation>
    </message>
    <message>
        <source>M&amp;inor Grid Spacing:</source>
        <translation type="obsolete">Крок &amp;допоміжної сітки:</translation>
    </message>
    <message>
        <source>Ma&amp;jor Grid Spacing:</source>
        <translation type="obsolete">Крок &amp;основної сітки:</translation>
    </message>
    <message>
        <source>Guide &amp;Snap Distance:</source>
        <translation type="obsolete">Відстань прив&apos;язки до нап&amp;равляючих:</translation>
    </message>
    <message>
        <source>Min&amp;or Grid Color:</source>
        <translation type="obsolete">Колір &amp;допоміжної сітки:</translation>
    </message>
    <message>
        <source>Majo&amp;r Grid Color:</source>
        <translation type="obsolete">Колір &amp;основної сітки:</translation>
    </message>
    <message>
        <source>&amp;User Guides Color:</source>
        <translation type="obsolete">Колір &amp;направляючих користувача:</translation>
    </message>
    <message>
        <source>Base&amp;line Grid Color:</source>
        <translation type="obsolete">Колір &amp;базової сітки:</translation>
    </message>
    <message>
        <source>In the &amp;Background</source>
        <translation type="obsolete">На &amp;фоні</translation>
    </message>
    <message>
        <source>In the Fore&amp;ground</source>
        <translation type="obsolete">На пере&amp;дньому плані</translation>
    </message>
    <message>
        <source>O&amp;n</source>
        <translation type="obsolete">&amp;Ввімкнено</translation>
    </message>
    <message>
        <source>O&amp;ff</source>
        <translation type="obsolete">В&amp;имкнено</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="obsolete">Змі&amp;щення:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="obsolete">&amp;Масштабування:</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="obsolete">Змі&amp;щення:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="obsolete">&amp;Масштабування:</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="obsolete">&amp;Масштабування:</translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="obsolete">&amp;Базова сітка:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="obsolete">Зс&amp;ув базової сітки:</translation>
    </message>
    <message>
        <source>Automatic &amp;Line Spacing:</source>
        <translation type="obsolete">Автоматичний інтервал мі&amp;ж рядками:</translation>
    </message>
    <message>
        <source>Default &amp;Font:</source>
        <translation type="obsolete">Шрифт по &amp;умовчанню:</translation>
    </message>
    <message>
        <source>Default &amp;Size:</source>
        <translation type="obsolete">Розмір по &amp;умовчанню:</translation>
    </message>
    <message>
        <source>&amp;Text Color:</source>
        <translation type="obsolete">&amp;Колір тексту:</translation>
    </message>
    <message>
        <source>Colu&amp;mns:</source>
        <translation type="obsolete">&amp;Стовпці:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="obsolete">&amp;Проміжок:</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="obsolete">Колір &amp;лінії:</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="obsolete">&amp;Затінення:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="obsolete">Колір запо&amp;внення:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="obsolete">&amp;Затінення:</translation>
    </message>
    <message>
        <source>&amp;Type of Line:</source>
        <translation type="obsolete">&amp;Тип лінії:</translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="obsolete">Тов&amp;щина лінії:</translation>
    </message>
    <message>
        <source>Mi&amp;nimum:</source>
        <translation type="obsolete">&amp;Мінімум:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="obsolete">Ма&amp;ксимум:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="obsolete">&amp;Крок:</translation>
    </message>
    <message>
        <source>&amp;Free Scaling</source>
        <translation type="obsolete">&amp;Вільне масштабування</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="obsolete">&amp;Горизонтальне масштабування:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="obsolete">&amp;Вертикальне масштабування:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="obsolete">Установити масштаб зображення по розміру &amp;рамки</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="obsolete">Зберігати &amp;співвідношення сторін</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="obsolete">Колір запо&amp;внення:</translation>
    </message>
    <message>
        <source>Corn&amp;ers:</source>
        <translation type="obsolete">К&amp;ути:</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="obsolete">&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>&amp;Factor:</source>
        <translation type="obsolete">&amp;Фактор:</translation>
    </message>
    <message>
        <source>Sa&amp;ve Contents on Changes</source>
        <translation>З&amp;берегти вміст при зміні</translation>
    </message>
    <message>
        <source>Large</source>
        <translation>Великий</translation>
    </message>
    <message>
        <source>Display Pages &amp;Side by Side</source>
        <translation type="obsolete">Показувати сторінки &amp;поруч</translation>
    </message>
    <message>
        <source>Page Colors</source>
        <translation type="obsolete">Кольори сторінок</translation>
    </message>
    <message>
        <source>&amp;Background:</source>
        <translation type="obsolete">&amp;Фон:</translation>
    </message>
    <message>
        <source>&amp;Margins:</source>
        <translation type="obsolete">&amp;Поля:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation>Виділити &amp;недрукуєму область кольором поля</translation>
    </message>
    <message>
        <source>Use PDF 1.4 &amp;Transparency Features</source>
        <translation type="obsolete">Використовувати про&amp;зорість PDF 1.4</translation>
    </message>
    <message>
        <source>&amp;Adjust Display Size</source>
        <translation>Поправити &amp;розмір для показу</translation>
    </message>
    <message>
        <source>&amp;Name of Executable:</source>
        <translation>Назва файла про&amp;грами:</translation>
    </message>
    <message>
        <source>Antialias &amp;Text</source>
        <translation>Антиаліасинг &amp;тексту</translation>
    </message>
    <message>
        <source>Antialias &amp;Graphics</source>
        <translation>Антиаліасинг &amp;графіки</translation>
    </message>
    <message>
        <source>Name of &amp;Executable:</source>
        <translation>Назва файла про&amp;грами:</translation>
    </message>
    <message>
        <source>Clip to Page &amp;Margins</source>
        <translation>Обрізати по поля&amp;х сторінки</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Зсередини:</translation>
    </message>
    <message>
        <source>O&amp;utside:</source>
        <translation type="obsolete">Зз&amp;овні:</translation>
    </message>
    <message>
        <source>Apply &amp;Under Color Removal</source>
        <translation>Виконати видалення &amp;кольорів</translation>
    </message>
    <message>
        <source>T&amp;emplates:</source>
        <translation type="obsolete">&amp;Шаблони:</translation>
    </message>
    <message>
        <source>Cha&amp;nge...</source>
        <translation>З&amp;мінити...</translation>
    </message>
    <message>
        <source>Apply &amp;Factor</source>
        <translation type="obsolete">Застосувати &amp;фактор</translation>
    </message>
    <message>
        <source>Additional Directory for Document Templates</source>
        <translation type="obsolete">Додаткова директорія для шаблонів документів</translation>
    </message>
    <message>
        <source>Apply Convex/Concave Factor to change shape of Polygons</source>
        <translation type="obsolete">Застосувати випуклий/ввігнутий фактор для зміни форми полігонів</translation>
    </message>
    <message>
        <source>A negative value will make the polygon concave (or star shaped),
 a positive value will make it convex</source>
        <translation type="obsolete">Негативне число зробить полігон ввігнутим (або зіркоподібним),
позитивне число зробить його випуклим</translation>
    </message>
    <message>
        <source>A way of switching off some of the gray shades which are composed
of cyan, yellow and magenta and using black instead.
UCR most affects parts of images which are neutral and/or dark tones
which are close to the gray. Use of this may improve printing some images
and some experimentation and testing is need on a case by case basis.
UCR reduces the possibility of over saturation with CMY inks.</source>
        <translation>Метод видалення певних відтінків сірого, які складаються з салатового, 
жовтого та рожевого кольорів, і використання чорного кольору на їх місці.
UCR найчастіше впливає на ті частини зображень, які складаються з нейтральних
та/чи темних відтінків близьких до сірого кольору. Використання цієї установки може
покращити якість друку деяких зображень. Деяке тестування та експериментування
необхідні в кожному конкретному випадку. UCR знижує ймовірність перенасичення
паперу CMY чорнилами.</translation>
    </message>
    <message>
        <source>Executive</source>
        <translation type="obsolete">Виконавчий</translation>
    </message>
    <message>
        <source>Folio</source>
        <translation type="obsolete">Альбомний</translation>
    </message>
    <message>
        <source>Ledger</source>
        <translation type="obsolete">Бухгалтерський</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation type="obsolete">Юридичний</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation type="obsolete">Лист</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation type="obsolete">Таблоїд</translation>
    </message>
    <message>
        <source>Default ICC profiles directory. This cannot
be changed with documents open.</source>
        <translation type="obsolete">Директорія профілів кольору ICC по умовчанню.
Ця установка не може бути змінена при наявності
відчинених документі.</translation>
    </message>
    <message>
        <source>Turns on the base grid</source>
        <translation type="obsolete">Вмикає базову сітку</translation>
    </message>
    <message>
        <source>Turns off the base grid</source>
        <translation type="obsolete">Вимикає базову сітку</translation>
    </message>
    <message>
        <source>File system location for the Ghostscript interpreter</source>
        <translation type="obsolete">Розміщення інтерпретатора Ghostscript в файловій системі</translation>
    </message>
    <message>
        <source>File system location for graphics editor. If you use gimp
and your distro includes it, we recommend &apos;gimp-remote&apos;,
as it allows you to edit the image in an already running
instance of gimp.</source>
        <translation>Розміщення графічного редактора. Якщо Ви використовуєте
Gimp і Ваш дистрибутив його включає, ми рекомендуємо
використання &apos;gimp-remote&apos;, так як воно дозволить Вам
редагувати зображення у вже запущеному екземплярі Gimp.</translation>
    </message>
    <message>
        <source>&amp;Language:</source>
        <translation type="unfinished">&amp;Мова:</translation>
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
        <translation type="unfinished">Колір:</translation>
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
        <translation type="obsolete">Тип</translation>
    </message>
    <message>
        <source>File</source>
        <translation type="obsolete">Файл</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation type="obsolete">Так</translation>
    </message>
    <message>
        <source>No</source>
        <translation type="obsolete">Ні</translation>
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
        <source>Default ICC profiles directory. This cannot
be changed with a document open.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Additional directory for document templates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Set the length of the action history in steps.
If set to 0 infinite amount of actions will be stored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Place a ruler against your screen and drag the slider to set the zoom level so Scribus will display your pages and objects on them at the correct size</source>
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
</context>
<context>
    <name>PrefsDialogBase</name>
    <message>
        <source>&amp;Defaults</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PrefsManager</name>
    <message>
        <source>Single Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Double sided</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page</source>
        <translation type="unfinished">Ліва сторінка</translation>
    </message>
    <message>
        <source>Right Page</source>
        <translation type="unfinished">Права сторінка</translation>
    </message>
    <message>
        <source>3-Fold</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Middle</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>4-Fold</source>
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
        <source>Postscript</source>
        <translation type="unfinished">Постскрипт</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Book</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Magazine</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You can select predefined page layout here. &apos;None&apos; leave margins as is, &apos;Book&apos; sets margins classically (Gutenberg). &apos;Book&apos; is proposed for two-sided documents. &apos;Magazine&apos; sets all margins for same value. Leading is Left/Inside value.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PythonConsole</name>
    <message>
        <source>&amp;Open...</source>
        <translation type="unfinished">&amp;Відчинити...</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Записати</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="unfinished">Записати &amp;як...</translation>
    </message>
    <message>
        <source>&amp;Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation type="unfinished">&amp;Файл</translation>
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
        <translation type="unfinished">Консоль сценарія</translation>
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
        <source>Open File With Python Commands</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save the Python Commands in File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Застереження</translation>
    </message>
    <message>
        <source>Text Files (*.txt)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save Current Output</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QColorDialog</name>
    <message>
        <source>Hu&amp;e:</source>
        <translation>&amp;Тон:</translation>
    </message>
    <message>
        <source>&amp;Sat:</source>
        <translation>На&amp;сиченість:</translation>
    </message>
    <message>
        <source>&amp;Val:</source>
        <translation>З&amp;начення:</translation>
    </message>
    <message>
        <source>&amp;Red:</source>
        <translation>&amp;Червоний:</translation>
    </message>
    <message>
        <source>&amp;Green:</source>
        <translation>&amp;Зелений:</translation>
    </message>
    <message>
        <source>Bl&amp;ue:</source>
        <translation>&amp;Синій:</translation>
    </message>
    <message>
        <source>A&amp;lpha channel:</source>
        <translation>&amp;Альфа канал:</translation>
    </message>
    <message>
        <source>&amp;Basic colors</source>
        <translation>&amp;Базові кольори</translation>
    </message>
    <message>
        <source>&amp;Custom colors</source>
        <translation>&amp;Власні кольори</translation>
    </message>
    <message>
        <source>&amp;Define Custom Colors &gt;&gt;</source>
        <translation>&amp;Вибрати власні кольори &gt;&gt;</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>&amp;Add to Custom Colors</source>
        <translation>&amp;Добавити у власні кольори</translation>
    </message>
    <message>
        <source>Select color</source>
        <translation>Вибрати колір</translation>
    </message>
</context>
<context>
    <name>QFileDialog</name>
    <message>
        <source>Copy or Move a File</source>
        <translation>Копіювати або перемістити файл</translation>
    </message>
    <message>
        <source>Read: %1</source>
        <translation>Зчитати: %1</translation>
    </message>
    <message>
        <source>Write: %1</source>
        <translation>Записати: %1</translation>
    </message>
    <message>
        <source>File &amp;name:</source>
        <translation>Ім&apos;я &amp;файла:</translation>
    </message>
    <message>
        <source>File &amp;type:</source>
        <translation>Тип &amp;файла:</translation>
    </message>
    <message>
        <source>One directory up</source>
        <translation>На один рівень вгору</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Всі файли (*)</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ім&apos;я</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Розмір</translation>
    </message>
    <message>
        <source>Type</source>
        <translation>Тип</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
    <message>
        <source>Attributes</source>
        <translation>Аттрибути</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Look &amp;in:</source>
        <translation>Шукати &amp;в:</translation>
    </message>
    <message>
        <source>Back</source>
        <translation>Назад</translation>
    </message>
    <message>
        <source>Create New Folder</source>
        <translation>Створити нову директорію</translation>
    </message>
    <message>
        <source>List View</source>
        <translation>Короткий список</translation>
    </message>
    <message>
        <source>Detail View</source>
        <translation>Детальний список</translation>
    </message>
    <message>
        <source>Preview File Info</source>
        <translation>Попередній перегляд інформації по файлу</translation>
    </message>
    <message>
        <source>Preview File Contents</source>
        <translation>Попередній перегляд вмісту файла</translation>
    </message>
    <message>
        <source>Read-write</source>
        <translation>Зчитування та запис</translation>
    </message>
    <message>
        <source>Read-only</source>
        <translation>Лише зчитування</translation>
    </message>
    <message>
        <source>Write-only</source>
        <translation>Лише запис</translation>
    </message>
    <message>
        <source>Inaccessible</source>
        <translation>Недоступний</translation>
    </message>
    <message>
        <source>Symlink to File</source>
        <translation>Символічний лінк до файла</translation>
    </message>
    <message>
        <source>Symlink to Directory</source>
        <translation>Символічний лінк до директорії</translation>
    </message>
    <message>
        <source>Symlink to Special</source>
        <translation>Символічний лінк до особливого</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Dir</source>
        <translation>Директорія</translation>
    </message>
    <message>
        <source>Special</source>
        <translation>Особливий</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Save As</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Відчинити</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation>&amp;Записати</translation>
    </message>
    <message>
        <source>&amp;Rename</source>
        <translation>&amp;Перейменувати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>R&amp;eload</source>
        <translation>П&amp;еречитати</translation>
    </message>
    <message>
        <source>Sort by &amp;Name</source>
        <translation>Сортувати по &amp;Імені</translation>
    </message>
    <message>
        <source>Sort by &amp;Size</source>
        <translation>Сортувати по &amp;Розміру</translation>
    </message>
    <message>
        <source>Sort by &amp;Date</source>
        <translation>Сортувати по &amp;Даті</translation>
    </message>
    <message>
        <source>&amp;Unsorted</source>
        <translation>&amp;Несортований</translation>
    </message>
    <message>
        <source>Sort</source>
        <translation>Сортувати</translation>
    </message>
    <message>
        <source>Show &amp;hidden files</source>
        <translation>Показати при&amp;ховані файли</translation>
    </message>
    <message>
        <source>the file</source>
        <translation>файл</translation>
    </message>
    <message>
        <source>the directory</source>
        <translation>директорія</translation>
    </message>
    <message>
        <source>the symlink</source>
        <translation>символічний лінк</translation>
    </message>
    <message>
        <source>Delete %1</source>
        <translation>Видалити %1</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Are you sure you wish to delete %1 &quot;%2&quot;?&lt;/qt&gt;</source>
        <translation>&lt;qt&gt;Ви справді бажаєте видалити %1 &quot;%2&quot;?&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&amp;Yes</source>
        <translation>&amp;Так</translation>
    </message>
    <message>
        <source>&amp;No</source>
        <translation>&amp;Ні</translation>
    </message>
    <message>
        <source>New Folder 1</source>
        <translation>Нова директорія 1</translation>
    </message>
    <message>
        <source>New Folder</source>
        <translation>Нова директорія</translation>
    </message>
    <message>
        <source>New Folder %1</source>
        <translation>Нова директорія %1</translation>
    </message>
    <message>
        <source>Find Directory</source>
        <translation>Знайти директорію</translation>
    </message>
    <message>
        <source>Directories</source>
        <translation>Директорії</translation>
    </message>
    <message>
        <source>Save</source>
        <translation>Записати</translation>
    </message>
    <message>
        <source>Error</source>
        <translation>Помилка</translation>
    </message>
    <message>
        <source>%1
File not found.
Check path and filename.</source>
        <translation>%1
Файл не знайдено.
Перевірте шлях та ім&apos;я файла.</translation>
    </message>
    <message>
        <source>All Files (*.*)</source>
        <translation>Всі файли (*.*)</translation>
    </message>
    <message>
        <source>Select a Directory</source>
        <translation>Вибрати директорію</translation>
    </message>
    <message>
        <source>Directory:</source>
        <translation>Директорія:</translation>
    </message>
</context>
<context>
    <name>QFontDialog</name>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Шрифт</translation>
    </message>
    <message>
        <source>Font st&amp;yle</source>
        <translation>&amp;Стиль шрифта</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Розмір</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation>Ефекти</translation>
    </message>
    <message>
        <source>Stri&amp;keout</source>
        <translation>Пере&amp;креслення</translation>
    </message>
    <message>
        <source>&amp;Underline</source>
        <translation>&amp;Підкреслення</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Колір</translation>
    </message>
    <message>
        <source>Sample</source>
        <translation>Зразок</translation>
    </message>
    <message>
        <source>Scr&amp;ipt</source>
        <translation>С&amp;ценарій</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Застосувати</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation>Вихід</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Зачинити</translation>
    </message>
    <message>
        <source>Select Font</source>
        <translation>Вибрати шрифт</translation>
    </message>
</context>
<context>
    <name>QLineEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очистити</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Вибрати все</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Відмінити</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Повторити</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Ви&amp;різати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Вклеїти</translation>
    </message>
</context>
<context>
    <name>QMainWindow</name>
    <message>
        <source>Line up</source>
        <translation>Вирівняти</translation>
    </message>
    <message>
        <source>Customize...</source>
        <translation>Свої установки...</translation>
    </message>
</context>
<context>
    <name>QMessageBox</name>
    <message>
        <source>&lt;h3&gt;About Qt&lt;/h3&gt;&lt;p&gt;This program uses Qt version %1.&lt;/p&gt;&lt;p&gt;Qt is a C++ toolkit for multiplatform GUI &amp;amp; application development.&lt;/p&gt;&lt;p&gt;Qt provides single-source portability across MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Linux, and all major commercial Unix variants.&lt;br&gt;Qt is also available for embedded devices.&lt;/p&gt;&lt;p&gt;Qt is a Trolltech product. See &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; for more information.&lt;/p&gt;</source>
        <translation>&lt;h3&gt;Про Qt&lt;/h3&gt;&lt;p&gt;Ця програма використовує версію %1 Qt .&lt;/p&gt;&lt;p&gt;Qt - багатоплатформний набір для розробки графічних оболонок та програмного забезпечення в C++.&lt;/p&gt;&lt;p&gt;Qt дає можливість портативного використання коду між  MS&amp;nbsp;Windows, Mac&amp;nbsp;OS&amp;nbsp;X, Лінукс та всіма основними комерційними варіантами UNIX.&lt;br&gt;Існує версія Qt  для вбудованих пристроїв.&lt;/p&gt;&lt;p&gt;Qt - продукт компанії Trolltech. Зверніться до &lt;tt&gt;http://www.trolltech.com/qt/&lt;/tt&gt; за додатковою інформацією.&lt;/p&gt;</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>Initializing...</source>
        <translation>Ініціалізація...</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Do you really want to overwrite the File:
%1 ?</source>
        <translation>Ви справді бажаєте переписати файл:
%1 ?</translation>
    </message>
    <message>
        <source>Print Preview</source>
        <translation type="obsolete">Перегляд  перед друком</translation>
    </message>
    <message>
        <source>Online Reference</source>
        <translation type="obsolete">Документація на Інтернеті</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Сценарії на Пітоні (*.py);; Всі файли (*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg *.svgz);;All Files (*)</source>
        <translation>SVG-зображення (*.svg *.svgz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>SVG-Images (*.svg);;All Files (*)</source>
        <translation>SVG-зображення (*.svg);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Так</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ні</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="obsolete">Документ</translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">С&amp;ценарії</translation>
    </message>
    <message>
        <source>Oook! You&apos;re calling an object doesn&apos;t exist!</source>
        <translation type="obsolete">Ойойой! Спроба доступу до неіснуючого об&apos;єкту!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to erase an object doesn&apos;t exist!</source>
        <translation type="obsolete">Ойойой! Спроба видалення неіснуючого об&apos;єкту!</translation>
    </message>
    <message>
        <source>Oook! An object you&apos;re trying to textflow doesn&apos;t exist!</source>
        <translation type="obsolete">Ойойой! Спроба оточення неіснуючого об&apos;єкту текстом!</translation>
    </message>
    <message>
        <source>Error writing the output file(s).</source>
        <translation>Помилка при запису вихідного файла(ів).</translation>
    </message>
    <message>
        <source>File exists. Overwrite?</source>
        <translation>Файл вже існує. Переписати поверх?</translation>
    </message>
    <message>
        <source>exists already. Overwrite?</source>
        <translation>вже існує. Переписати поверх?</translation>
    </message>
    <message>
        <source>Yes all</source>
        <translation>Гаразд для всіх</translation>
    </message>
    <message>
        <source>Save as Image</source>
        <translation>Записати, як зображення</translation>
    </message>
    <message>
        <source>Error writting the output file(s).</source>
        <translation type="obsolete">Помилка при запису файла(ів).</translation>
    </message>
    <message>
        <source>Export successful.</source>
        <translation>Експорт успішний.</translation>
    </message>
    <message>
        <source>All Supported Formats (*.eps *.EPS *.ps *.PS);;</source>
        <translation>Всі доступні формати (*.eps *.EPS *.ps *.PS);;</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Всі файли (*)</translation>
    </message>
    <message>
        <source>Newsletters</source>
        <translation>Інформаційні бюлетні</translation>
    </message>
    <message>
        <source>Brochures</source>
        <translation>Брошури</translation>
    </message>
    <message>
        <source>Catalogs</source>
        <translation>Каталоги</translation>
    </message>
    <message>
        <source>Flyers</source>
        <translation>Листівки</translation>
    </message>
    <message>
        <source>Signs</source>
        <translation>Знаки</translation>
    </message>
    <message>
        <source>Cards</source>
        <translation>Картки</translation>
    </message>
    <message>
        <source>Letterheads</source>
        <translation>Бланки</translation>
    </message>
    <message>
        <source>Envelopes</source>
        <translation>Конверти</translation>
    </message>
    <message>
        <source>Business Cards</source>
        <translation>Візитні картки</translation>
    </message>
    <message>
        <source>Calendars</source>
        <translation>Календарі</translation>
    </message>
    <message>
        <source>Advertisements</source>
        <translation>Реклами</translation>
    </message>
    <message>
        <source>Labels</source>
        <translation>Етикетки</translation>
    </message>
    <message>
        <source>Menus</source>
        <translation>Меню</translation>
    </message>
    <message>
        <source>Programs</source>
        <translation>Програми</translation>
    </message>
    <message>
        <source>PDF Forms</source>
        <translation>PDF Форми</translation>
    </message>
    <message>
        <source>Magazines</source>
        <translation>Журнали</translation>
    </message>
    <message>
        <source>Posters</source>
        <translation>Плакати</translation>
    </message>
    <message>
        <source>Announcements</source>
        <translation>Оголошення</translation>
    </message>
    <message>
        <source>Text Documents</source>
        <translation>Текстові документи</translation>
    </message>
    <message>
        <source>Folds</source>
        <translation>Розгортки</translation>
    </message>
    <message>
        <source>Own Templates</source>
        <translation>Шаблони користувача</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Попередній перегляд шрифтів</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Вставити символ</translation>
    </message>
    <message>
        <source>New &amp;from Template...</source>
        <translation type="obsolete">Новий з &amp;шаблона...</translation>
    </message>
    <message>
        <source>PDF Presentations</source>
        <translation>PDF презентації</translation>
    </message>
    <message>
        <source>Save as &amp;Image...</source>
        <translation type="obsolete">Записати, як зо&amp;браження...</translation>
    </message>
    <message>
        <source>Print Previe&amp;w</source>
        <translation type="obsolete">Перегляд перед д&amp;руком</translation>
    </message>
    <message>
        <source>Import &amp;EPS/PS...</source>
        <translation type="obsolete">Імпорт &amp;ЕPS/PS...</translation>
    </message>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="obsolete">Записати як ша&amp;блон...</translation>
    </message>
    <message>
        <source>S&amp;cripter Manual...</source>
        <translation type="obsolete">Керівництво по &amp;сценаристу...</translation>
    </message>
    <message>
        <source>&amp;Scribus Scripts</source>
        <translation type="unfinished">С&amp;ценарії Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="unfinished">&amp;Виконати сценарій...</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="unfinished">&amp;Недавно виконані сценарії</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="unfinished">Покзати ко&amp;нсоль</translation>
    </message>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="obsolete">Записати, як SVG з&amp;ображення...</translation>
    </message>
    <message>
        <source>Import &amp;SVG...</source>
        <translation type="obsolete">Імпортувати  SVG зображ&amp;ення...</translation>
    </message>
    <message>
        <source>Oook! Wrong arguments! Call: </source>
        <translation type="obsolete">Ойойой! Неправильні аргументи! Виклик: </translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to load image into an object doesn&apos;t exist or isn&apos;t selected!</source>
        <translation type="obsolete">Ойойой! Ви намагаєтеся завантажити зображення в об&apos;єкт який не існує, або не вибраний!</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to (un)lock an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Ойойой! Ви намагаєтеся замкнути/відімкнути неіснуючий об&apos;єкт! До того ж жодного з об&apos;єктів не вибрано.</translation>
    </message>
    <message>
        <source>Oook! You&apos;re trying to query an object doesn&apos;t exist! None selected too.</source>
        <translation type="obsolete">Ойойой! Ви намагаєтеся звернутися до неіснуючого об&apos;єкта! До того ж жодного об&apos;єкта не вибрано.</translation>
    </message>
    <message>
        <source>Importing text</source>
        <translation>Імпорт тексту</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Всі формати, що підтримуються</translation>
    </message>
    <message>
        <source>HTML Files</source>
        <translation>HTML файли</translation>
    </message>
    <message>
        <source>html</source>
        <translation>html</translation>
    </message>
    <message>
        <source>Text Files</source>
        <translation>Текстові файли</translation>
    </message>
    <message>
        <source>Comma Separated Value Files</source>
        <translation>Файли з розділеними комами полями</translation>
    </message>
    <message>
        <source>CSV_data</source>
        <translation>Дані_cvs</translation>
    </message>
    <message>
        <source>CSV_header</source>
        <translation>Заголовок_cvs</translation>
    </message>
    <message>
        <source>Font %1 is broken, discarding it</source>
        <translation>Видаляється пошкоджений шрифт %1</translation>
    </message>
    <message>
        <source>Template: </source>
        <translation type="obsolete">Шаблон:</translation>
    </message>
    <message>
        <source>
External Links
</source>
        <translation>Зовнішні зсилки</translation>
    </message>
    <message>
        <source>OO.o Writer Documents</source>
        <translation type="obsolete">Документи OO.o  Writer</translation>
    </message>
    <message>
        <source>Text Filters</source>
        <translation>Текстові фільтри</translation>
    </message>
    <message>
        <source>Media Cases</source>
        <translation>Футляри для мультимедійних носіїв</translation>
    </message>
    <message>
        <source>Albanian</source>
        <translation>Албанська</translation>
    </message>
    <message>
        <source>Basque</source>
        <translation>Баскська</translation>
    </message>
    <message>
        <source>Bulgarian</source>
        <translation>Болгарська</translation>
    </message>
    <message>
        <source>Brazilian</source>
        <translation>Бразільська</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Каталанська</translation>
    </message>
    <message>
        <source>Chinese</source>
        <translation>Китайська</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Чешська</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Датська</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Голландська</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Англійська</translation>
    </message>
    <message>
        <source>English (British)</source>
        <translation>Англійська (Британська)</translation>
    </message>
    <message>
        <source>Esperanto</source>
        <translation>Есперанто</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Німецька</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Фінська</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Французька</translation>
    </message>
    <message>
        <source>Galician</source>
        <translation>Галицька</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Грецька</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Венгерська</translation>
    </message>
    <message>
        <source>Indonesian</source>
        <translation>Індонезійська</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Італійська</translation>
    </message>
    <message>
        <source>Korean</source>
        <translation>Корейська</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Литовська</translation>
    </message>
    <message>
        <source>Norwegian (Bokmaal)</source>
        <translation>Норвезька (Букмааль)</translation>
    </message>
    <message>
        <source>Norwegian (Nnyorsk)</source>
        <translation>Норвезька (Нниорськ)</translation>
    </message>
    <message>
        <source>Norwegian</source>
        <translation>Норвезька</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Польська</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Російська</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Шведська</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Іспанська</translation>
    </message>
    <message>
        <source>Spanish (Latin)</source>
        <translation>Іспанська (Латинь)</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Словацька</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Словенська</translation>
    </message>
    <message>
        <source>Serbian</source>
        <translation>Сербська</translation>
    </message>
    <message>
        <source>Tried to set progress &gt; maximum progress</source>
        <translation type="obsolete">Була зроблена спроба установки показника 
прогресу більшого ніж максимально можливий</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="unfinished">&amp;Про скрипт...</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="unfinished">Про скрипт</translation>
    </message>
    <message>
        <source>Cannot get font size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити розмір шрифта в нетекстовій рамці.
</translation>
    </message>
    <message>
        <source>Cannot get font of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити шрифт в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot get text size of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити розмір тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot get column count of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити число колонок тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot get line space of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити міжрядковий інтервал в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot get column gap of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо визначити дистанцію між колонками тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot get text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо повернути текст з нетекстової рамки.</translation>
    </message>
    <message>
        <source>Cannot set text of non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити текст в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot insert text into non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо вставити текст в нетекстову рамку.</translation>
    </message>
    <message>
        <source>Insert index out of bounds</source>
        <comment>python error</comment>
        <translation type="obsolete">Індекс вставки виходить за доступний діапазон</translation>
    </message>
    <message>
        <source>Alignment out of range. Use one of the scribus.ALIGN* constants.</source>
        <comment>python error</comment>
        <translation>Вирівнювання виходить за доступний діапазон. Використайте
одну з констант ALIGN*.</translation>
    </message>
    <message>
        <source>Can&apos;t set text alignment on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити вирівнювання тексту в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512</source>
        <comment>python error</comment>
        <translation type="obsolete">Розмір шрифта виходить за доступний діапазон - має бути 1 &lt;= розмір &lt;= 512</translation>
    </message>
    <message>
        <source>Can&apos;t set font size on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити розмір шрифта в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can&apos;t set font on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити текст в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Font not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Шрифт не знайдено</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1</source>
        <comment>python error</comment>
        <translation type="obsolete">Міжрядковий інтервал виходить за доступний діапазон, має бути &gt;= 0.1</translation>
    </message>
    <message>
        <source>Can&apos;t line spacing on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо визначити міжрядковий інтервал в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive</source>
        <comment>python error</comment>
        <translation type="obsolete">Дистанція між колонками тексту виходить за доступний діапазон, має бути позитивною</translation>
    </message>
    <message>
        <source>Can&apos;t column gap on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити число колонок тексту в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1</source>
        <comment>python error</comment>
        <translation type="obsolete">Число колонок тексту виходить за доступний діапазон, має бути &gt; 1</translation>
    </message>
    <message>
        <source>Can&apos;t number of columns on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити число колонок тексту в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Selection index out of bounds</source>
        <comment>python error</comment>
        <translation>Індекс вибірки виходить за доступний діапазон</translation>
    </message>
    <message>
        <source>Can&apos;t select text in a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо вибрати текст в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can&apos;t delete text from a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо видалити текст з нетекстової рамки</translation>
    </message>
    <message>
        <source>Can&apos;t set text fill on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити заповнення тексту в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can&apos;t set text stroke on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити виділення контурів знаків в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can&apos;t set text shade on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити тінь тексту в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can only link text frames</source>
        <comment>python error</comment>
        <translation type="obsolete">Лише текстові рамки можуть бути з&apos;єднані в ланцюжок</translation>
    </message>
    <message>
        <source>Target frame must be empty</source>
        <comment>python error</comment>
        <translation type="obsolete">Рамка призначення має бути пуста</translation>
    </message>
    <message>
        <source>Target frame links to another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Рамка призначення зв&apos;язана з іншою рамкою</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Інша рамка зв&apos;язана з рамкою призначення</translation>
    </message>
    <message>
        <source>Source and target are the same object</source>
        <comment>python error</comment>
        <translation type="obsolete">Вихідна рамка і рамка призначення являються одним і тим самим об&apos;єктом</translation>
    </message>
    <message>
        <source>Can&apos;t unlink a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо від&apos;єднати нетекстову рамку</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, can&apos;t unlink.</source>
        <comment>python error</comment>
        <translation type="unfinished">Об&apos;єкт не являється зв&apos;язаною текстовою рамкою, неможливо розімкнути.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, can&apos;t unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="unfinished">Об&apos;єкт являється останньою рамкою в серії, неможливо роз&apos;єднати. Роз&apos;єднайте попередню рамку натомість.</translation>
    </message>
    <message>
        <source>Can&apos;t convert a non-text frame to outlines</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо конвертувати нетекстову рамку в контурні лінії</translation>
    </message>
    <message>
        <source>Failed to open document</source>
        <comment>python error</comment>
        <translation type="obsolete">Документ неможливо відкрити</translation>
    </message>
    <message>
        <source>Failed to save document</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо записати документ</translation>
    </message>
    <message>
        <source>Unit out of range. Use one of the scribus.UNIT_* constants.</source>
        <comment>python error</comment>
        <translation>Одиниця виходить за доступний діапазон. Використайте одну з
scribus.UNIT_* констант.</translation>
    </message>
    <message>
        <source>Target is not an image frame.</source>
        <comment>python error</comment>
        <translation>Рамка призначення не є рамкою для зображень.</translation>
    </message>
    <message>
        <source>Can&apos;t scale by 0%</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо змінити масштаб на 0%</translation>
    </message>
    <message>
        <source>Can&apos;t render an empty sample</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо прорисувати пустий зразок</translation>
    </message>
    <message>
        <source>Can&apos;t save to a blank filename</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо записати файл з пустим ім&apos;ям</translation>
    </message>
    <message>
        <source>Can&apos;t have an empty layer name</source>
        <comment>python error</comment>
        <translation type="obsolete">Не допускається пусте ім&apos;я плану</translation>
    </message>
    <message>
        <source>Layer not found</source>
        <comment>python error</comment>
        <translation type="obsolete">План не знайдено</translation>
    </message>
    <message>
        <source>Can&apos;t remove the last layer</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо видалити останній існуючий план</translation>
    </message>
    <message>
        <source>Can&apos;t create layer without a name</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо створити план без імені</translation>
    </message>
    <message>
        <source>An object with the requested name already exists</source>
        <comment>python error</comment>
        <translation type="obsolete">Об&apos;єкт з таким ім&apos;ям вже існує</translation>
    </message>
    <message>
        <source>Point list must contain at least two points (four values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Список точок має складатися, як мінімум, з двох точок (чотири значення)</translation>
    </message>
    <message>
        <source>Point list must contain an even number of values</source>
        <comment>python error</comment>
        <translation type="obsolete">Список точок має складатися з парного числа значень</translation>
    </message>
    <message>
        <source>Point list must contain at least three points (six values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Список точок має складатися, як мінімум, з трьох точок (шість значень)</translation>
    </message>
    <message>
        <source>Point list must contain at least four points (eight values)</source>
        <comment>python error</comment>
        <translation type="obsolete">Список точок має складатися, як мінімум, з чотирьох точок (вісім значень)</translation>
    </message>
    <message>
        <source>Point list must have a multiple of six values</source>
        <comment>python error</comment>
        <translation type="obsolete">Список точок має складатися з шестикратного числа значень</translation>
    </message>
    <message>
        <source>Object not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Об&apos;єкт не знайдено</translation>
    </message>
    <message>
        <source>Style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Стиль не знайдено</translation>
    </message>
    <message>
        <source>Can&apos;t set style on a non-text frame</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо установити стиль в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Failed to save EPS</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо записати EPS (енкапсульований постскрипт)</translation>
    </message>
    <message>
        <source>Page number out of range</source>
        <comment>python error</comment>
        <translation type="obsolete">Число сторінок виходить за доступний діапазон</translation>
    </message>
    <message>
        <source>argument is not list: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">аргумент не є списком: має бути списком дійсних чисел</translation>
    </message>
    <message>
        <source>argument contains non-numeric values: must be list of float values</source>
        <comment>python error</comment>
        <translation type="obsolete">аргумент містить нечислові значення: має бути списком дійсних чисел</translation>
    </message>
    <message>
        <source>Line width out of bounds, must be 0 &lt;= line_width &lt;= 12</source>
        <comment>python error</comment>
        <translation type="obsolete">Товщина лінії виходить за межі дозволеного, має бути  0 &lt;= товщина_лінії &lt;= 12</translation>
    </message>
    <message>
        <source>Line shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Тінь лінії виходить за межі дозволеного, має бути  0 &lt;= тінь &lt;= 100</translation>
    </message>
    <message>
        <source>Fill shade out of bounds, must be 0 &lt;= shade &lt;= 100</source>
        <comment>python error</comment>
        <translation type="obsolete">Тінь заповнення виходить за межі дозволеного, має бути  0 &lt;= тінь &lt;= 100</translation>
    </message>
    <message>
        <source>Corner radius must be a positive number.</source>
        <comment>python error</comment>
        <translation>Радіус кута повинен бути позитивним числом.</translation>
    </message>
    <message>
        <source>Line style not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Стиль лінії не знайдено</translation>
    </message>
    <message>
        <source>Cannot get a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Неможливо повернути колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Color not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Колір не знайдено</translation>
    </message>
    <message>
        <source>Cannot change a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Неможливо змінити колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Color not found in document</source>
        <comment>python error</comment>
        <translation type="obsolete">Колір не знайдено в документі</translation>
    </message>
    <message>
        <source>Color not found in default colors</source>
        <comment>python error</comment>
        <translation type="obsolete">Колір не знайдено в стандартних кольорах</translation>
    </message>
    <message>
        <source>Cannot create a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Неможливо створити колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Cannot delete a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Неможливо видалити колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Cannot replace a color with an empty name.</source>
        <comment>python error</comment>
        <translation>Неможливо замінити колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Scribus Python interface module
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>This module is the Python interface for Scribus. It provides functions
<byte value="x9"/><byte value="x9"/>to control scribus and to manipulate objects on the canvas. Each
<byte value="x9"/><byte value="x9"/>function is documented individually below.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>A few things are common across most of the interface.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>Most functions operate on frames. Frames are identified by their name,
<byte value="x9"/><byte value="x9"/>a string - they are not real Python objects. Many functions take an
<byte value="x9"/><byte value="x9"/>optional (non-keyword) parameter, a frame name.
<byte value="x9"/><byte value="x9"/>Many exceptions are also common across most functions. These are
<byte value="x9"/><byte value="x9"/>not currently documented in the docstring for each function.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>    - Many functions will raise a NoDocOpenError if you try to use them
<byte value="x9"/><byte value="x9"/>      without a document to operate on.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>    - If you do not pass a frame name to a function that requires one,
<byte value="x9"/><byte value="x9"/>      the function will use the currently selected frame, if any, or
<byte value="x9"/><byte value="x9"/>      raise a NoValidObjectError if it can&apos;t find anything to operate
<byte value="x9"/><byte value="x9"/>      on.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>    - Many functions will raise WrongFrameTypeError if you try to use them
<byte value="x9"/><byte value="x9"/>      on a frame type that they do not make sense with. For example, setting
<byte value="x9"/><byte value="x9"/>      the text colour on a graphics frame doesn&apos;t make sense, and will result
<byte value="x9"/><byte value="x9"/>      in this exception being raised.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>    - Errors resulting from calls to the underlying Python API will be
<byte value="x9"/><byte value="x9"/>      passed through unaltered. As such, the list of exceptions thrown by
<byte value="x9"/><byte value="x9"/>      any function as provided here and in its docstring is incomplete.
<byte value="x9"/><byte value="x9"/><byte value="x9"/><byte value="x9"/>Details of what exceptions each function may throw are provided on the
<byte value="x9"/><byte value="x9"/>function&apos;s documentation.
<byte value="x9"/><byte value="x9"/></source>
        <translation type="obsolete">Модуль інтерфейса Scribus для мови програмування Пітон (Python).

Цей модуль є інтерфейсом Scribus для Пітона. Він містить функції, які дозволяють
керувати програмою scribus та маніпулювати об&apos;єктами на канві. Кожна функція
має окреме описання. Декілька понять є загальними для більшої частини інтерфейса.
Більшість функцій оперують рамками. Рамки визначаються за їх назвою, яка
виражається строковою величиною, вони не є справжніми об&apos;єктами Пітону. Багато
функій сприймають необов&apos;язковий параметр (не ключове слово) - назва рамки.
Багато помилок (винятків) також є спільними для більшості функцій.
- Багато функцій повернуть помилку NoDocOpenError, якщо їх виконати при відсутності
відчиненого документа, яким вони могли б оперувати.
- Якщо не передати назву рамки функції, яка потребує цей параметр, ця функція 
використає вибрану рамку, якщо така є, або поверне помилку NoValidObjectError, якщо
вона не зможе знайти відповідний об&apos;єкт.
- Багато функцій повернуть помилку WrongFrameTypeError, якщо їх використати на 
невірному типі рамки. Наприклад установка кольору тексту для рамки для зображень
не має сенсу і приведе до повернення цієї помилки.
- Помилки, які повертаються викликами до базового програмного інтерфейсу Пітона (Python API) будуть повертатися без змін. Це приводить то того, що список помилок, які
можуть повертатися любою функцією, як вказано тут і в документаційній строці функції, є неповним. Довідка про помилки, які можуть бути повернені кожною функцією розміщена
в документації по функції.</translation>
    </message>
    <message>
        <source>Import &amp;OpenOffice.org Draw...</source>
        <translation type="obsolete">Імпортувати файл  &amp;OpenOffice.org Draw...</translation>
    </message>
    <message>
        <source>OpenOffice.org Draw (*.sxd);;All Files (*)</source>
        <translation>OpenOffice.org Draw (*.sxd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Documents</source>
        <translation>Документи OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Color not found - python error</source>
        <comment>python error</comment>
        <translation>Колір не знайдено - помилка Пітону</translation>
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
        <translation type="obsolete">Модуль взаємодії Пітону та Scribus

Цей модуль служить, як інтерфейс між Пітоном та Scribus. Він містить
функції для контролю над scribus та для маніпуляції об&apos;єктами на канві.
Кожна функція окремо описана нижче.

Кілька загальних положень для цього інтерфейсу.

Більшість функції оперують над рамками. Рамки визначаються за їх 
іменами, які є строковими величинами, а не справжніми об&apos;єктами Пітону.
Багато функцій приймають додатковий (не ключове слово) параметр -
ім&apos;я рамки. 
Багато помилок також є загальними для більшості функцій. Вони не 
вказані в документаційній строці кожної функції.
- Багато функції повернуть помилку NoDocOpenError, якщо Ви спробуєте
використати їх при відсутності відчиненого документа.
- Якщо Ви не передасте ім&apos;я рамки функції, яка його вимагає, ця функція
використає вибрану рамку, якщо така існує, або поверне помилку 
NoValidObjectError, якщо не зможе знайти жодного об&apos;єкта для маніпулювання.
- Багато функцій повернуть помилку WrongFrameTypeError, якщо Ви спробуєте
використати їх на невідповідному типові рамки. Наприклад, зміна кольору тексту
в рамці зображень не має смислу і приведе до повернення цієї помилки.
- Помилки, що повертаються викликами до базового програмного інтерфейсу
Пітона будуть повернені без змін. Тому список можливих помилок для любої 
функції залишається неповним.

Деталі по помилках, які може повернути кожна функція надаються в
документації по цій функції.</translation>
    </message>
    <message>
        <source>Custom (optional) configuration: </source>
        <comment>short words plugin</comment>
        <translation>Нетипова (необов&apos;язкова) конфігурація:</translation>
    </message>
    <message>
        <source>Standard configuration: </source>
        <comment>short words plugin</comment>
        <translation>Стандартна конфігурація:</translation>
    </message>
    <message>
        <source>Short &amp;Words...</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">Короткі &amp;Слова...</translation>
    </message>
    <message>
        <source>Short Words processing. Wait please...</source>
        <comment>short words plugin</comment>
        <translation>Опрацювання коротких слів. Будь-ласка почекайте...</translation>
    </message>
    <message>
        <source>Short Words processing. Done.</source>
        <comment>short words plugin</comment>
        <translation>Опрацювання коротких слів закінчено.</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Африкаанс</translation>
    </message>
    <message>
        <source>Turkish</source>
        <translation>Турецька</translation>
    </message>
    <message>
        <source>Ukranian</source>
        <translation>Українська</translation>
    </message>
    <message>
        <source>Welsh</source>
        <translation>Уельська</translation>
    </message>
    <message>
        <source>The filename must be a string.</source>
        <comment>python error</comment>
        <translation>Назва файла повинна бути строковою величиною.</translation>
    </message>
    <message>
        <source>Cannot delete image type settings.</source>
        <comment>python error</comment>
        <translation>Не можу видалити установки типу зображення.</translation>
    </message>
    <message>
        <source>The image type must be a string.</source>
        <comment>python error</comment>
        <translation>Тип зображення повинен бути строковою величиною.</translation>
    </message>
    <message>
        <source>&apos;allTypes&apos; attribute is READ-ONLY</source>
        <comment>python error</comment>
        <translation>Атрибут &apos;allTypes&apos; може бути лише зчитаним</translation>
    </message>
    <message>
        <source>Failed to export image</source>
        <comment>python error</comment>
        <translation>Не вдалося експортувати зображення</translation>
    </message>
    <message>
        <source>Color not found.</source>
        <comment>python error</comment>
        <translation>Колір не знайдено.</translation>
    </message>
    <message>
        <source>Color not found in document.</source>
        <comment>python error</comment>
        <translation>Колір не знайдено в документі.</translation>
    </message>
    <message>
        <source>Color not found in default colors.</source>
        <comment>python error</comment>
        <translation>Колір не знайдено в стандартних кольорах.</translation>
    </message>
    <message>
        <source>Cannot scale by 0%.</source>
        <comment>python error</comment>
        <translation>Змінити масштаб на 0% неможливо.</translation>
    </message>
    <message>
        <source>Specified item not an image frame.</source>
        <comment>python error</comment>
        <translation>Вказаний об&apos;єкт не являється рамкою для зображень.</translation>
    </message>
    <message>
        <source>Font not found.</source>
        <comment>python error</comment>
        <translation>Шрифт не знайдено.</translation>
    </message>
    <message>
        <source>Cannot render an empty sample.</source>
        <comment>python error</comment>
        <translation>Неможливо заповнити пустий зразок.</translation>
    </message>
    <message>
        <source>Cannot have an empty layer name.</source>
        <comment>python error</comment>
        <translation>Неможливо використати пусте ім&apos;я плану.</translation>
    </message>
    <message>
        <source>Layer not found.</source>
        <comment>python error</comment>
        <translation>План не знайдено.</translation>
    </message>
    <message>
        <source>Cannot remove the last layer.</source>
        <comment>python error</comment>
        <translation>Неможливо видалити останній план.</translation>
    </message>
    <message>
        <source>Cannot create layer without a name.</source>
        <comment>python error</comment>
        <translation>Неможливо створити план без імені.</translation>
    </message>
    <message>
        <source>Insert index out of bounds.</source>
        <comment>python error</comment>
        <translation>Індекс вставки виходить за доступний діапазон.</translation>
    </message>
    <message>
        <source>Cannot set text alignment on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо встановити вирівнювання тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Font size out of bounds - must be 1 &lt;= size &lt;= 512.</source>
        <comment>python error</comment>
        <translation>Розмір шрифта виходить за доступний діапазон - повинен бути 1 &lt;= розмір &lt;= 512.</translation>
    </message>
    <message>
        <source>Cannot set font size on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити розмір шрифта в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot set font on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити шрифт в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Line space out of bounds, must be &gt;= 0.1.</source>
        <comment>python error</comment>
        <translation>Міжрядковий інтервал виходить за межі дозволеного, має бути  &gt;= 0.1.</translation>
    </message>
    <message>
        <source>Cannot set line spacing on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити міжрядковий інтервал в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Column gap out of bounds, must be positive.</source>
        <comment>python error</comment>
        <translation>Дистанція між колонками тексту виходить за дозволений проміжок. Повинна бути позитивною.</translation>
    </message>
    <message>
        <source>Cannot set column gap on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити дистанцію між колонками тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Column count out of bounds, must be &gt; 1.</source>
        <comment>python error</comment>
        <translation>Число колонок тексту виходить за доступний діапазон - повинен бути &gt; 1.</translation>
    </message>
    <message>
        <source>Cannot set number of columns on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити число колонок тексту в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot select text in a non-text frame</source>
        <comment>python error</comment>
        <translation>Неможливо вибрати текст в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Cannot delete text from a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо видалити текст в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot set text fill on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити заповнення текста в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot set text stroke on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити контур текста в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Cannot set text shade on a non-text frame.</source>
        <comment>python error</comment>
        <translation>Неможливо установити тінь текста в нетекстовій рамці.</translation>
    </message>
    <message>
        <source>Can only link text frames.</source>
        <comment>python error</comment>
        <translation>Установити зв&apos;язок можна лише між текстовими рамками.</translation>
    </message>
    <message>
        <source>Target frame must be empty.</source>
        <comment>python error</comment>
        <translation>Рамка призначення повинна бути пустою.</translation>
    </message>
    <message>
        <source>Target frame links to another frame.</source>
        <comment>python error</comment>
        <translation>Рамка призначення зв&apos;язана з іншою рамкою.</translation>
    </message>
    <message>
        <source>Target frame is linked to by another frame.</source>
        <comment>python error</comment>
        <translation>Інша рамка зв&apos;язана з рамкою призначення.</translation>
    </message>
    <message>
        <source>Source and target are the same object.</source>
        <comment>python error</comment>
        <translation>Джерело та ціль є одним і тим же об&apos;єктом.</translation>
    </message>
    <message>
        <source>Cannot unlink a non-text frame.</source>
        <comment>python error</comment>
        <translation>Видалити зв&apos;язок можна лише між текстовими рамками.</translation>
    </message>
    <message>
        <source>Object is not a linked text frame, cannot unlink.</source>
        <comment>python error</comment>
        <translation type="obsolete">Об&apos;єкт не являється зв&apos;язаною текстовою рамкою, неможливо від&apos;єднати.</translation>
    </message>
    <message>
        <source>Object the last frame in a series, cannot unlink. Unlink the previous frame instead.</source>
        <comment>python error</comment>
        <translation type="obsolete">Об&apos;єкт являється останньою рамков в ряду - неможливо від&apos;єднати. Від&apos;єднайте
попередню рамку натомість.</translation>
    </message>
    <message>
        <source>Cannot convert a non-text frame to outlines.</source>
        <comment>python error</comment>
        <translation>Неможливо конвертувати нетекстову рамку в контур.</translation>
    </message>
    <message>
        <source>Portuguese (Brazilian)</source>
        <translation type="obsolete">Португальська (Бразильська)</translation>
    </message>
    <message>
        <source>Cannot get a colour with an empty name.</source>
        <comment>python error</comment>
        <translation type="obsolete">Неможливо визначити колір з пустим ім&apos;ям.</translation>
    </message>
    <message>
        <source>Colour not found</source>
        <comment>python error</comment>
        <translation type="obsolete">Колір не знайдено</translation>
    </message>
    <message>
        <source>Unable to save pixmap.</source>
        <comment>scripter error</comment>
        <translation type="obsolete">Неможливо записати растрове зображення.</translation>
    </message>
    <message>
        <source>Can&apos;t set bookmark on a non-text frame</source>
        <comment>python error</comment>
        <translation>Неможливо установити закладку в нетекстовій рамці</translation>
    </message>
    <message>
        <source>Can&apos;t get info from a non-text frame</source>
        <comment>python error</comment>
        <translation>Неможливо визначити інформацію з нетекстової рамки</translation>
    </message>
    <message>
        <source>OpenDocument Text Documents</source>
        <translation>Текстові документи OpenDocument </translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Хорватська</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation type="unfinished">Португальська</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation type="unfinished">Португальська (Бр)</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="unfinished">Аварійний вихід Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="unfinished">Аварійний вихід Scribus із-за сигнала №%1</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="unfinished">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation type="unfinished">Нестандартний</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Сторінка</translation>
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
        <source>You are running a development version of Scribus 1.3.x. The current document you are working with was originally created in Scribus 1.2.2 or lower. The process of saving will make this file unusable again in Scribus 1.2.2 unless you use File-&gt;Save As. Are you sure you wish to proceed with this operation?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Proceed</source>
        <translation type="unfinished"></translation>
    </message>
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
        <source>Uses xx as shortcut for a language</source>
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
        <source>Scribus Version </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus, Open Source Desktop Publishing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>---------------------------------------</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Homepage:       http://www.scribus.net </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documentation:  http://docs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Wiki:           http://wiki.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Issues:         http://bugs.scribus.net</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="unfinished">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="unfinished">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">тчк</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="unfinished">мм</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="unfinished">д</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="unfinished">п</translation>
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
        <translation type="unfinished">Точки (тчк)</translation>
    </message>
    <message>
        <source>Millimeters (mm)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Inches (in)</source>
        <translation type="unfinished">Дюйми (д)</translation>
    </message>
    <message>
        <source>Picas (p)</source>
        <translation type="unfinished">Піки (п)</translation>
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
        <source>A file named &apos;%1&apos; already exists.
Do you want to replace it with the file you are saving?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation type="unfinished">&amp;Заміна</translation>
    </message>
    <message>
        <source>page</source>
        <comment>page export</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation type="obsolete">&amp;Попередній перегляд шрифтів...</translation>
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
        <source>&amp;Script</source>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Copy #%1 of </source>
        <translation type="unfinished">Копія №%1 з </translation>
    </message>
    <message>
        <source>Black</source>
        <translation type="unfinished">Чорний</translation>
    </message>
    <message>
        <source>Cyan</source>
        <translation type="unfinished">Салатовий</translation>
    </message>
    <message>
        <source>Magenta</source>
        <translation type="unfinished">Малиновий</translation>
    </message>
    <message>
        <source>Yellow</source>
        <translation type="unfinished">Жовтий</translation>
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
        <translation type="unfinished">Новий з шаблона</translation>
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
        <translation type="unfinished">Короткі слова</translation>
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
</context>
<context>
    <name>QTextEdit</name>
    <message>
        <source>Clear</source>
        <translation>Очистити</translation>
    </message>
    <message>
        <source>Select All</source>
        <translation>Вибрати все</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation>&amp;Відмінити</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation>&amp;Повторити</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>&amp;Вирізати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Вклеїти</translation>
    </message>
</context>
<context>
    <name>QTitleBar</name>
    <message>
        <source>System Menu</source>
        <translation>Системне меню</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Затінити</translation>
    </message>
    <message>
        <source>Unshade</source>
        <translation>Відтінити</translation>
    </message>
    <message>
        <source>Normalize</source>
        <translation>Нормальний розмір</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Згорнути до мінімуму</translation>
    </message>
    <message>
        <source>Maximize</source>
        <translation>Розгорнути до максимуму</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Зачинити</translation>
    </message>
</context>
<context>
    <name>QWorkspace</name>
    <message>
        <source>&amp;Restore</source>
        <translation>&amp;Відновити</translation>
    </message>
    <message>
        <source>&amp;Move</source>
        <translation>&amp;Перемістити</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Розмір</translation>
    </message>
    <message>
        <source>Mi&amp;nimize</source>
        <translation>Зверну&amp;ти</translation>
    </message>
    <message>
        <source>Ma&amp;ximize</source>
        <translation>Ма&amp;ксимальний розмір</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Зачинити</translation>
    </message>
    <message>
        <source>Stay on &amp;Top</source>
        <translation>&amp;Залишатися поверх</translation>
    </message>
    <message>
        <source>Minimize</source>
        <translation>Звернути</translation>
    </message>
    <message>
        <source>Restore Down</source>
        <translation>Відновити вниз</translation>
    </message>
    <message>
        <source>Close</source>
        <translation>Зачинити</translation>
    </message>
    <message>
        <source>Sh&amp;ade</source>
        <translation>За&amp;тінити</translation>
    </message>
    <message>
        <source>%1 - [%2]</source>
        <translation>%1 - [%2]</translation>
    </message>
    <message>
        <source>&amp;Unshade</source>
        <translation>Відті&amp;нити</translation>
    </message>
</context>
<context>
    <name>Query</name>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>ReformDoc</name>
    <message>
        <source>Document Setup</source>
        <translation>Установка опцій документа</translation>
    </message>
    <message>
        <source>Margin Guides</source>
        <translation>Розмітка полів</translation>
    </message>
    <message>
        <source>Enable single or spread based layout</source>
        <translation type="obsolete">Вибрати одиничний чи книжковий перепліт</translation>
    </message>
    <message>
        <source>Make the first page the left page of the document</source>
        <translation type="obsolete">Помістити першу сторінку документу зліва</translation>
    </message>
    <message>
        <source>Distance between the top margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі верхнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the bottom margin guide and the edge of the page</source>
        <translation type="obsolete">Відстань від границі нижнього поля сторінки до її краю</translation>
    </message>
    <message>
        <source>Distance between the left margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі лівого поля сторінки до її краю. Якщо вибраний 
книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source>Distance between the right margin guide and the edge of the page.
If Facing Pages is selected, this margin space can be used to achieve the correct margins for binding</source>
        <translation type="obsolete">Відстань від границі правого поля сторінки до її краю. Якщо вибраний 
книжковий перепліт, то цей проміжок може використовуватися
для установки правильних полів для переплітання</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>&amp;Top:</source>
        <translation>&amp;Верх:</translation>
    </message>
    <message>
        <source>&amp;Left:</source>
        <translation>&amp;Лівий край:</translation>
    </message>
    <message>
        <source>&amp;Bottom:</source>
        <translation>&amp;Низ:</translation>
    </message>
    <message>
        <source>&amp;Right:</source>
        <translation>&amp;Правий край:</translation>
    </message>
    <message>
        <source>&amp;Facing Pages</source>
        <translation type="obsolete">&amp;Суміжні сторінки</translation>
    </message>
    <message>
        <source>Left &amp;Page First</source>
        <translation type="obsolete">&amp;Ліва сторінка перша</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Inside:</source>
        <translation type="obsolete">&amp;Зсередини:</translation>
    </message>
    <message>
        <source>&amp;Outside:</source>
        <translation type="obsolete">Зз&amp;овні:</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Розмір сторінки</translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="obsolete">Розмір:</translation>
    </message>
    <message>
        <source>Custom</source>
        <translation>Нестандартний</translation>
    </message>
    <message>
        <source>Orientation:</source>
        <translation type="obsolete">Орієнтація:</translation>
    </message>
    <message>
        <source>Portrait</source>
        <translation>Вертикальна</translation>
    </message>
    <message>
        <source>Landscape</source>
        <translation>Горизонтальна</translation>
    </message>
    <message>
        <source>Width:</source>
        <translation type="obsolete">Товщина:</translation>
    </message>
    <message>
        <source>Height:</source>
        <translation type="obsolete">Висота:</translation>
    </message>
    <message>
        <source>F&amp;irst Page Number:</source>
        <translation>Номер &amp;першої сторінки:</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation type="unfinished">&amp;Розмір:</translation>
    </message>
    <message>
        <source>Orie&amp;ntation:</source>
        <translation type="unfinished">&amp;Орієнтація:</translation>
    </message>
    <message>
        <source>&amp;Width:</source>
        <translation type="unfinished">&amp;Ширина:</translation>
    </message>
    <message>
        <source>&amp;Height:</source>
        <translation type="unfinished">&amp;Висота:</translation>
    </message>
    <message>
        <source>&amp;Unit:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Options</source>
        <translation type="unfinished">Установки</translation>
    </message>
    <message>
        <source>Apply size settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Apply margin settings to all Pages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Autosave</source>
        <translation type="unfinished">Автоматичне збереження</translation>
    </message>
    <message>
        <source>min</source>
        <translation type="unfinished">хв</translation>
    </message>
    <message>
        <source>&amp;Interval:</source>
        <translation type="unfinished">&amp;Інтервал:</translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Документ</translation>
    </message>
    <message>
        <source>Document Information</source>
        <translation type="unfinished">Інформація про документ</translation>
    </message>
    <message>
        <source>Guides</source>
        <translation type="unfinished">Розмітка</translation>
    </message>
    <message>
        <source>Page Display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Колір:</translation>
    </message>
    <message>
        <source>Display &amp;Unprintable Area in Margin Color</source>
        <translation type="unfinished">Виділити &amp;недрукуєму область кольором поля</translation>
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
        <translation type="unfinished">Відображення</translation>
    </message>
    <message>
        <source>Typography</source>
        <translation type="unfinished">Типографія</translation>
    </message>
    <message>
        <source>Tools</source>
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
        <translation type="unfinished">Колір паперу</translation>
    </message>
    <message>
        <source>Mask the area outside the margins in the margin color</source>
        <translation type="unfinished">Замаскувати область за полями кольором полів</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="unfinished">Настройка кольорів</translation>
    </message>
    <message>
        <source>Enable or disable the display of linked text frames.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RunScriptDialog</name>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="unfinished">Сценарії на Пітоні (*.py);; Всі файли (*)</translation>
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
        <source>Style of current paragraph</source>
        <translation>Стиль активного абзацу</translation>
    </message>
    <message>
        <source>Style Settings</source>
        <translation>Установки стилю</translation>
    </message>
</context>
<context>
    <name>SToolBColorF</name>
    <message>
        <source>None</source>
        <translation>Ніякого</translation>
    </message>
    <message>
        <source>Color of text fill</source>
        <translation>Колір заповнення тексту</translation>
    </message>
    <message>
        <source>Saturation of color of text fill</source>
        <translation>Насиченість кольору заповнення тексту</translation>
    </message>
    <message>
        <source>Fill Color Settings</source>
        <translation>Установки кольору заповнення</translation>
    </message>
</context>
<context>
    <name>SToolBColorS</name>
    <message>
        <source>None</source>
        <translation>Ніякого</translation>
    </message>
    <message>
        <source>Color of text stroke</source>
        <translation>Колір силуетів тексту</translation>
    </message>
    <message>
        <source>Saturation of color of text stroke</source>
        <translation>Насиченість кольору силуетів тексту</translation>
    </message>
    <message>
        <source>Stroke Color Settings</source>
        <translation>Установки кольору контурів</translation>
    </message>
</context>
<context>
    <name>SToolBFont</name>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Font of selected text</source>
        <translation>Шрифт вибраного тексту</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Розмір шрифта</translation>
    </message>
    <message>
        <source>Scaling width of characters</source>
        <translation>Масштабування ширини знаків</translation>
    </message>
    <message>
        <source>Font Settings</source>
        <translation>Установки шрифтів</translation>
    </message>
    <message>
        <source>Scaling height of characters</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SToolBStyle</name>
    <message>
        <source>Kerning:</source>
        <translation type="obsolete">Кернінг:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>Manual Kerning</source>
        <translation type="obsolete">Ручна обробка</translation>
    </message>
    <message>
        <source>Character Settings</source>
        <translation>Установки текстових символів</translation>
    </message>
    <message>
        <source>Tracking:</source>
        <translation type="obsolete">Слідкування:</translation>
    </message>
    <message>
        <source>Manual Tracking</source>
        <translation>Слідкування вручну</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
</context>
<context>
    <name>SVGExportPlugin</name>
    <message>
        <source>Save Page as &amp;SVG...</source>
        <translation type="unfinished">Записати, як SVG з&amp;ображення...</translation>
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
        <translation type="unfinished">Імпортувати  SVG зображ&amp;ення...</translation>
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
</context>
<context>
    <name>SaveAsTemplatePlugin</name>
    <message>
        <source>Save as &amp;Template...</source>
        <translation type="unfinished">Записати як ша&amp;блон...</translation>
    </message>
</context>
<context>
    <name>ScActionPlugin</name>
    <message>
        <source>Could not find target file %1: %2</source>
        <comment>plugins</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Could not open target file %1: %2</source>
        <comment>plugins</comment>
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
        <translation type="unfinished">Імпортувати</translation>
    </message>
    <message>
        <source>Export</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Action</source>
        <comment>plugin manager plugin type</comment>
        <translation type="unfinished">Дія</translation>
    </message>
</context>
<context>
    <name>ScriXmlDoc</name>
    <message>
        <source>Copy #%1 of </source>
        <translation>Копія №%1 з </translation>
    </message>
    <message>
        <source>Background</source>
        <translation>Фон</translation>
    </message>
</context>
<context>
    <name>ScribusApp</name>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Searching for Fonts</source>
        <translation>Пошук шрифтів</translation>
    </message>
    <message>
        <source>There are no Postscript-Fonts on your System</source>
        <translation type="obsolete">На Вашій системі немає Postscript шрифтів</translation>
    </message>
    <message>
        <source>Exiting now</source>
        <translation type="obsolete">Виходжу зараз</translation>
    </message>
    <message>
        <source>Fatal Error</source>
        <translation>Фатальна помилка</translation>
    </message>
    <message>
        <source>Smart Hyphen</source>
        <translation type="obsolete">Розумний дефіс</translation>
    </message>
    <message>
        <source>Align Left</source>
        <translation type="obsolete">Вирівняти по лівому краю</translation>
    </message>
    <message>
        <source>Align Right</source>
        <translation type="obsolete">Вирівняти по правому краю</translation>
    </message>
    <message>
        <source>Align Center</source>
        <translation type="obsolete">Вирівняти по центру</translation>
    </message>
    <message>
        <source>Insert Page Number</source>
        <translation type="obsolete">Вставити номер сторінки</translation>
    </message>
    <message>
        <source>Attach Text to Path</source>
        <translation type="obsolete">Приєднати текст до шляху</translation>
    </message>
    <message>
        <source>Show Layers</source>
        <translation type="obsolete">Показати плани</translation>
    </message>
    <message>
        <source>Javascripts...</source>
        <translation type="obsolete">Сценарії Javascript...</translation>
    </message>
    <message>
        <source>Undo</source>
        <translation type="obsolete">Відмінити</translation>
    </message>
    <message>
        <source>Show Page Palette</source>
        <translation type="obsolete">Показати палітру сторінки</translation>
    </message>
    <message>
        <source>Lock/Unlock</source>
        <translation type="obsolete">Замкнути/Відімкнути</translation>
    </message>
    <message>
        <source>Non Breaking Space</source>
        <translation type="obsolete">Пропуск без розриву</translation>
    </message>
    <message>
        <source>Reading Preferences</source>
        <translation>Зчитування установок</translation>
    </message>
    <message>
        <source>Init Hyphenator</source>
        <translation type="obsolete">Ініціалізувати переніс</translation>
    </message>
    <message>
        <source>Setting up Shortcuts</source>
        <translation>Установка гарячих клавіш</translation>
    </message>
    <message>
        <source>Reading Scrapbook</source>
        <translation>Зчитування чорновика</translation>
    </message>
    <message>
        <source>Initializing Plugins</source>
        <translation>Ініціалізація модулів</translation>
    </message>
    <message>
        <source>New</source>
        <translation type="obsolete">Новий</translation>
    </message>
    <message>
        <source>Open...</source>
        <translation type="obsolete">Відчинити...</translation>
    </message>
    <message>
        <source>Close</source>
        <translation type="obsolete">Зачинити</translation>
    </message>
    <message>
        <source>Save</source>
        <translation type="obsolete">Записати</translation>
    </message>
    <message>
        <source>Save as...</source>
        <translation type="obsolete">Записати як...</translation>
    </message>
    <message>
        <source>Get Text/Picture...</source>
        <translation type="obsolete">Вставити текст/зображення... </translation>
    </message>
    <message>
        <source>Document Info...</source>
        <translation type="obsolete">Інформація про документ...</translation>
    </message>
    <message>
        <source>Document Setup...</source>
        <translation type="obsolete">Установка опцій документу...</translation>
    </message>
    <message>
        <source>Print...</source>
        <translation type="obsolete">Друк...</translation>
    </message>
    <message>
        <source>Quit</source>
        <translation type="obsolete">Вихід</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="obsolete">Вирізати</translation>
    </message>
    <message>
        <source>Copy</source>
        <translation type="obsolete">Копіювати</translation>
    </message>
    <message>
        <source>Paste</source>
        <translation type="obsolete">Вклеїти</translation>
    </message>
    <message>
        <source>Clear</source>
        <translation type="obsolete">Очистити</translation>
    </message>
    <message>
        <source>Select all</source>
        <translation type="obsolete">Виділити все</translation>
    </message>
    <message>
        <source>Colors...</source>
        <translation type="obsolete">Кольори...</translation>
    </message>
    <message>
        <source>Styles...</source>
        <translation type="obsolete">Стилі...</translation>
    </message>
    <message>
        <source>Templates...</source>
        <translation type="obsolete">Шаблони...</translation>
    </message>
    <message>
        <source>Fonts...</source>
        <translation type="obsolete">Шрифти...</translation>
    </message>
    <message>
        <source>Select New Font</source>
        <translation type="obsolete">Вибрати новий шрифт</translation>
    </message>
    <message>
        <source>Duplicate</source>
        <translation type="obsolete">Дублювати</translation>
    </message>
    <message>
        <source>Multiple Duplicate</source>
        <translation type="obsolete">Багаторазове дублювання</translation>
    </message>
    <message>
        <source>Delete</source>
        <translation type="obsolete">Видалити</translation>
    </message>
    <message>
        <source>Group</source>
        <translation type="obsolete">Згрупувати</translation>
    </message>
    <message>
        <source>Un-group</source>
        <translation type="obsolete">Розгрупувати</translation>
    </message>
    <message>
        <source>Lock</source>
        <translation type="obsolete">Замкнути</translation>
    </message>
    <message>
        <source>Send to Back</source>
        <translation type="obsolete">Опустити на задній план</translation>
    </message>
    <message>
        <source>Bring to Front</source>
        <translation type="obsolete">Підняти на передній план</translation>
    </message>
    <message>
        <source>Lower</source>
        <translation type="obsolete">Опустити нижче</translation>
    </message>
    <message>
        <source>Raise</source>
        <translation type="obsolete">Підняти вище</translation>
    </message>
    <message>
        <source>Distribute/Align...</source>
        <translation type="obsolete">Розташувати/Вирівняти...</translation>
    </message>
    <message>
        <source>Insert...</source>
        <translation type="obsolete">Вставити...</translation>
    </message>
    <message>
        <source>Delete...</source>
        <translation type="obsolete">Видалити...</translation>
    </message>
    <message>
        <source>Move...</source>
        <translation type="obsolete">Пересунути...</translation>
    </message>
    <message>
        <source>Apply Template...</source>
        <translation type="obsolete">Застосувати шаблон...</translation>
    </message>
    <message>
        <source>Manage Guides...</source>
        <translation type="obsolete">Установка розмітки...</translation>
    </message>
    <message>
        <source>Fit in Window</source>
        <translation type="obsolete">Вмістити у вікно</translation>
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
        <source>200%</source>
        <translation type="obsolete">200%</translation>
    </message>
    <message>
        <source>Thumbnails</source>
        <translation type="obsolete">Мініатюрні зображення</translation>
    </message>
    <message>
        <source>Hide Margins</source>
        <translation type="obsolete">Сховати поля</translation>
    </message>
    <message>
        <source>Hide Frames</source>
        <translation type="obsolete">Сховати рамки</translation>
    </message>
    <message>
        <source>Hide Images</source>
        <translation type="obsolete">Сховати зображення</translation>
    </message>
    <message>
        <source>Show Grid</source>
        <translation type="obsolete">Показати сітку</translation>
    </message>
    <message>
        <source>Snap to Grid</source>
        <translation type="obsolete">Притягування до сітки</translation>
    </message>
    <message>
        <source>Tools</source>
        <translation type="obsolete">Інструменти</translation>
    </message>
    <message>
        <source>Properties</source>
        <translation type="obsolete">Властивості</translation>
    </message>
    <message>
        <source>Outline</source>
        <translation type="obsolete">Схема документу</translation>
    </message>
    <message>
        <source>Scrapbook</source>
        <translation type="obsolete">Чорновик</translation>
    </message>
    <message>
        <source>Manage Pictures</source>
        <translation type="obsolete">Керування зображеннями</translation>
    </message>
    <message>
        <source>Hyphenate Text</source>
        <translation type="obsolete">Перенос тексту</translation>
    </message>
    <message>
        <source>About Scribus</source>
        <translation type="obsolete">Про програму Scribus</translation>
    </message>
    <message>
        <source>About Qt</source>
        <translation>Про систему Qt</translation>
    </message>
    <message>
        <source>Online-Help...</source>
        <translation type="obsolete">Внутрішня довідка ...</translation>
    </message>
    <message>
        <source>Style</source>
        <translation type="obsolete">Стиль</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Нормальний</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="obsolete">Підкреслення</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="obsolete">Перекреслення</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="obsolete">Капітель</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="obsolete">Верхній індекс</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation type="obsolete">Нижній індекс</translation>
    </message>
    <message>
        <source>Outlined</source>
        <translation type="obsolete">Обведення</translation>
    </message>
    <message>
        <source>X-Pos:</source>
        <translation>Поз. Х:</translation>
    </message>
    <message>
        <source>Y-Pos:</source>
        <translation>Поз. У:</translation>
    </message>
    <message>
        <source>Ready</source>
        <translation>Готовий</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
    <message>
        <source>Get Picture...</source>
        <translation type="obsolete">Вставити зображення...</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="obsolete">Колір</translation>
    </message>
    <message>
        <source>Invert</source>
        <translation type="obsolete">Негатив</translation>
    </message>
    <message>
        <source>Get Text...</source>
        <translation type="obsolete">Вставити текст...</translation>
    </message>
    <message>
        <source>Font</source>
        <translation type="obsolete">Шрифт</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Розмір</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Тінь</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="obsolete">Відімкнути</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation type="obsolete">Документи (*.sla *.sla.gz *.scd *.scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Loading...</source>
        <translation>Завантаження...</translation>
    </message>
    <message>
        <source>All Files (*)</source>
        <translation>Всі файли (*)</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстові файли (*.txt);;Всі файли(*)</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Saving...</source>
        <translation>Записую...</translation>
    </message>
    <message>
        <source>Printing...</source>
        <translation>Друкую...</translation>
    </message>
    <message>
        <source>Document</source>
        <translation>Документ</translation>
    </message>
    <message>
        <source>Printing failed!</source>
        <translation>Не вийшло надрукувати!</translation>
    </message>
    <message>
        <source>Scribus Manual</source>
        <translation>Довідка по Scribus</translation>
    </message>
    <message>
        <source>The following Programs are missing:</source>
        <translation type="obsolete">Наступні програми відсутні:</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Все</translation>
    </message>
    <message>
        <source>EPS-Files (*.eps);;All Files (*)</source>
        <translation type="obsolete">EPS-файли (*.eps);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Loading:</source>
        <translation type="obsolete">Завантаження:</translation>
    </message>
    <message>
        <source>Adjusting Colors</source>
        <translation type="obsolete">Настройка кольорів</translation>
    </message>
    <message>
        <source>English</source>
        <translation>Англійська</translation>
    </message>
    <message>
        <source>German</source>
        <translation>Німецька</translation>
    </message>
    <message>
        <source>Spanish</source>
        <translation>Іспанська</translation>
    </message>
    <message>
        <source>Italian</source>
        <translation>Італійська</translation>
    </message>
    <message>
        <source>French</source>
        <translation>Французська</translation>
    </message>
    <message>
        <source>Russian</source>
        <translation>Російська</translation>
    </message>
    <message>
        <source>Danish</source>
        <translation>Датська</translation>
    </message>
    <message>
        <source>Slovak</source>
        <translation>Словацька</translation>
    </message>
    <message>
        <source>Hungarian</source>
        <translation>Венгерська</translation>
    </message>
    <message>
        <source>Czech</source>
        <translation>Чешська</translation>
    </message>
    <message>
        <source>Dutch</source>
        <translation>Голландська</translation>
    </message>
    <message>
        <source>Portuguese</source>
        <translation>Португальська</translation>
    </message>
    <message>
        <source>Ukrainian</source>
        <translation>Українська</translation>
    </message>
    <message>
        <source>Polish</source>
        <translation>Польська</translation>
    </message>
    <message>
        <source>Greek</source>
        <translation>Грецька</translation>
    </message>
    <message>
        <source>Catalan</source>
        <translation>Каталанська</translation>
    </message>
    <message>
        <source>Finnish</source>
        <translation>Фінська</translation>
    </message>
    <message>
        <source>Choose a Directory</source>
        <translation type="obsolete">Вибрати директорію</translation>
    </message>
    <message>
        <source>Scribus Crash</source>
        <translation type="obsolete">Аварійний вихід Scribus</translation>
    </message>
    <message>
        <source>Scribus crashes due to Signal #%1</source>
        <translation type="obsolete">Аварійний вихід Scribus із-за сигнала №%1</translation>
    </message>
    <message>
        <source>Irish</source>
        <translation>Ірландська</translation>
    </message>
    <message>
        <source>All Supported Formats</source>
        <translation>Всі формати, що підтримуються</translation>
    </message>
    <message>
        <source>Can&apos;t write the File: 
%1</source>
        <translation type="obsolete">Неможливо записати до Файла:
%1</translation>
    </message>
    <message>
        <source>Create a new Document</source>
        <translation type="obsolete">Створити новий документ</translation>
    </message>
    <message>
        <source>Open a Document</source>
        <translation type="obsolete">Відчинити документ</translation>
    </message>
    <message>
        <source>Save the current Document</source>
        <translation type="obsolete">Записати активний документ</translation>
    </message>
    <message>
        <source>Close the current Document</source>
        <translation type="obsolete">Зачинити активний документ</translation>
    </message>
    <message>
        <source>Print the current Document</source>
        <translation type="obsolete">Друкувати активний документ</translation>
    </message>
    <message>
        <source>Save the current Document as PDF</source>
        <translation type="obsolete">Записати активний документ як PDF</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагування</translation>
    </message>
    <message>
        <source>&amp;Item</source>
        <translation>&amp;Об&apos;єкт</translation>
    </message>
    <message>
        <source>&amp;Page</source>
        <translation>С&amp;торінка</translation>
    </message>
    <message>
        <source>&amp;View</source>
        <translation>&amp;Вид</translation>
    </message>
    <message>
        <source>&amp;Tools</source>
        <translation>&amp;Інструменти</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation>&amp;Додатки</translation>
    </message>
    <message>
        <source>&amp;Windows</source>
        <translation>Ві&amp;кна</translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation>До&amp;помога</translation>
    </message>
    <message>
        <source>Some Objects are locked.</source>
        <translation type="obsolete">Деякі об&apos;єкти замкнені.</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="obsolete">Вихід</translation>
    </message>
    <message>
        <source>Lock all</source>
        <translation type="obsolete">Замкнути все</translation>
    </message>
    <message>
        <source>Unlock all</source>
        <translation type="obsolete">Розімкнути все</translation>
    </message>
    <message>
        <source>Show Baseline Grid</source>
        <translation type="obsolete">Показати базову сітку</translation>
    </message>
    <message>
        <source>Hide Baseline Grid</source>
        <translation type="obsolete">Сховати базову сітку</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>Lithuanian</source>
        <translation>Литовська</translation>
    </message>
    <message>
        <source>Swedish</source>
        <translation>Шведська</translation>
    </message>
    <message>
        <source>Slovenian</source>
        <translation>Словенська</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation type="obsolete">&amp;Установки</translation>
    </message>
    <message>
        <source>&amp;Color Management...</source>
        <translation type="obsolete">&amp;Керування палітрою &amp;кольорів...</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation type="obsolete">&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Open...</source>
        <translation type="obsolete">&amp;Відчинити...</translation>
    </message>
    <message>
        <source>Open &amp;Recent</source>
        <translation>Відчинити &amp;Недавно редагований документ</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Зачинити</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Записати</translation>
    </message>
    <message>
        <source>Save &amp;As...</source>
        <translation type="obsolete">Записати &amp;як...</translation>
    </message>
    <message>
        <source>Re&amp;vert to Saved</source>
        <translation type="obsolete">Повернутися до за&amp;писаного</translation>
    </message>
    <message>
        <source>Collect for O&amp;utput...</source>
        <translation type="obsolete">Зібрати для в&amp;иводу...</translation>
    </message>
    <message>
        <source>&amp;Get Text/Picture...</source>
        <translation type="obsolete">В&amp;ставити зображення...</translation>
    </message>
    <message>
        <source>Append &amp;Text...</source>
        <translation type="obsolete">Додати &amp;текст...</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation>І&amp;мпортувати</translation>
    </message>
    <message>
        <source>Save &amp;Text...</source>
        <translation type="obsolete">Записати &amp;текст...</translation>
    </message>
    <message>
        <source>Save Page as &amp;EPS...</source>
        <translation type="obsolete">Записати, як &amp;ЕPS...</translation>
    </message>
    <message>
        <source>Save as P&amp;DF...</source>
        <translation type="obsolete">Записати, як &amp;РDF...</translation>
    </message>
    <message>
        <source>&amp;Export</source>
        <translation>&amp;Експортування</translation>
    </message>
    <message>
        <source>Document &amp;Setup...</source>
        <translation type="obsolete">Установка &amp;опцій документу...</translation>
    </message>
    <message>
        <source>&amp;Print...</source>
        <translation type="obsolete">&amp;Друк...</translation>
    </message>
    <message>
        <source>&amp;Quit</source>
        <translation type="obsolete">&amp;Вихід</translation>
    </message>
    <message>
        <source>&amp;Undo</source>
        <translation type="obsolete">&amp;Відмінити</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation type="obsolete">Ви&amp;різати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation type="obsolete">&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="obsolete">&amp;Вклеїти</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">О&amp;чистити</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation type="obsolete">Виді&amp;лити все</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation type="obsolete">&amp;Пошук/Заміна...</translation>
    </message>
    <message>
        <source>C&amp;olors...</source>
        <translation type="obsolete">&amp;Кольори...</translation>
    </message>
    <message>
        <source>&amp;Paragraph Styles...</source>
        <translation type="obsolete">&amp;Стилі абзаців...</translation>
    </message>
    <message>
        <source>&amp;Line Styles...</source>
        <translation type="obsolete">Стилі &amp;ліній...</translation>
    </message>
    <message>
        <source>&amp;Templates...</source>
        <translation type="obsolete">&amp;Шаблони...</translation>
    </message>
    <message>
        <source>&amp;Javascripts...</source>
        <translation type="obsolete">С&amp;ценарії Javascript...</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation type="obsolete">&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Multiple Duplicate</source>
        <translation type="obsolete">&amp;Багаторазове дублювання</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="obsolete">&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Group</source>
        <translation type="obsolete">З&amp;групувати</translation>
    </message>
    <message>
        <source>&amp;Ungroup</source>
        <translation type="obsolete">&amp;Розгрупувати</translation>
    </message>
    <message>
        <source>&amp;Lock</source>
        <translation type="obsolete">Зам&amp;кнути</translation>
    </message>
    <message>
        <source>Send to &amp;Back</source>
        <translation type="obsolete">Опустити на &amp;задній план</translation>
    </message>
    <message>
        <source>Bring to &amp;Front</source>
        <translation type="obsolete">Підняти на &amp;передній план</translation>
    </message>
    <message>
        <source>&amp;Lower</source>
        <translation type="obsolete">&amp;Опустити</translation>
    </message>
    <message>
        <source>&amp;Raise</source>
        <translation type="obsolete">&amp;Підняти</translation>
    </message>
    <message>
        <source>Distribute/&amp;Align...</source>
        <translation type="obsolete">Розташувати/&amp;Вирівняти...</translation>
    </message>
    <message>
        <source>&amp;Shape</source>
        <translation>&amp;Фігура</translation>
    </message>
    <message>
        <source>&amp;Attach Text to Path</source>
        <translation type="obsolete">При&amp;єднати текст до шляху</translation>
    </message>
    <message>
        <source>&amp;Detach Text from Path</source>
        <translation type="obsolete">В&amp;ідєднати текст від шляху</translation>
    </message>
    <message>
        <source>&amp;Combine Polygons</source>
        <translation type="obsolete">Поєдн&amp;ати полігони</translation>
    </message>
    <message>
        <source>Split &amp;Polygons</source>
        <translation type="obsolete">Розділ&amp;ити полігони</translation>
    </message>
    <message>
        <source>C&amp;onvert to Outlines</source>
        <translation type="obsolete">&amp;Конвертувати в силуети</translation>
    </message>
    <message>
        <source>&amp;Insert...</source>
        <translation type="obsolete">&amp;Вставити...</translation>
    </message>
    <message>
        <source>&amp;Delete...</source>
        <translation type="obsolete">&amp;Видалити...</translation>
    </message>
    <message>
        <source>&amp;Move...</source>
        <translation type="obsolete">&amp;Перемістити...</translation>
    </message>
    <message>
        <source>&amp;Apply Template...</source>
        <translation type="obsolete">&amp;Застосувати шаблон...</translation>
    </message>
    <message>
        <source>&amp;Fit in Window</source>
        <translation type="obsolete">Вмістити у ві&amp;кно</translation>
    </message>
    <message>
        <source>&amp;100%</source>
        <translation type="obsolete">&amp;100%</translation>
    </message>
    <message>
        <source>&amp;Thumbnails</source>
        <translation type="obsolete">&amp;Мініатюрні зображення</translation>
    </message>
    <message>
        <source>Show &amp;Grid</source>
        <translation type="obsolete">Показати &amp;сітку</translation>
    </message>
    <message>
        <source>Sna&amp;p to Guides</source>
        <translation type="obsolete">Притягування до розмітк&amp;и</translation>
    </message>
    <message>
        <source>Show &amp;Baseline Grid</source>
        <translation type="obsolete">Показати &amp;базову сітку</translation>
    </message>
    <message>
        <source>&amp;Properties</source>
        <translation type="obsolete">&amp;Властивості</translation>
    </message>
    <message>
        <source>&amp;Outline</source>
        <translation type="obsolete">С&amp;хема документу</translation>
    </message>
    <message>
        <source>&amp;Scrapbook</source>
        <translation type="obsolete">&amp;Чорновик</translation>
    </message>
    <message>
        <source>&amp;Layers</source>
        <translation type="obsolete">П&amp;лани</translation>
    </message>
    <message>
        <source>P&amp;age Palette</source>
        <translation type="obsolete">&amp;Палітра сторінки</translation>
    </message>
    <message>
        <source>&amp;Bookmarks</source>
        <translation type="obsolete">&amp;Закладки</translation>
    </message>
    <message>
        <source>&amp;Manage Pictures</source>
        <translation type="obsolete">Керування &amp;зображеннями</translation>
    </message>
    <message>
        <source>&amp;Hyphenate Text</source>
        <translation type="obsolete">Пе&amp;ренос тексту</translation>
    </message>
    <message>
        <source>Toolti&amp;ps</source>
        <translation type="obsolete">Пі&amp;дказки</translation>
    </message>
    <message>
        <source>P&amp;DF Tools</source>
        <translation type="obsolete">PDF &amp;Інструменти</translation>
    </message>
    <message>
        <source>Tooltips</source>
        <translation type="obsolete">Підказки</translation>
    </message>
    <message>
        <source>&amp;Fonts...</source>
        <translation type="obsolete">&amp;Шрифти...</translation>
    </message>
    <message>
        <source>&amp;Hyphenator...</source>
        <translation type="obsolete">Пе&amp;ренесення...</translation>
    </message>
    <message>
        <source>&amp;Keyboard Shortcuts...</source>
        <translation type="obsolete">&amp;Гарячі клавіші...</translation>
    </message>
    <message>
        <source>&amp;About Scribus</source>
        <translation type="obsolete">&amp;Про програму Scribus</translation>
    </message>
    <message>
        <source>About &amp;Qt</source>
        <translation type="obsolete">Про &amp;систему Qt</translation>
    </message>
    <message>
        <source>Scribus &amp;Manual...</source>
        <translation type="obsolete">&amp;Довідка по Scribus...</translation>
    </message>
    <message>
        <source>St&amp;yle</source>
        <translation>&amp;Стиль</translation>
    </message>
    <message>
        <source>&amp;Left</source>
        <translation type="obsolete">&amp;Ліве</translation>
    </message>
    <message>
        <source>&amp;Center</source>
        <translation type="obsolete">По &amp;центру</translation>
    </message>
    <message>
        <source>&amp;Right</source>
        <translation type="obsolete">&amp;Праве</translation>
    </message>
    <message>
        <source>&amp;Block</source>
        <translation type="obsolete">&amp;Блок</translation>
    </message>
    <message>
        <source>&amp;Forced</source>
        <translation type="obsolete">&amp;Вимушене</translation>
    </message>
    <message>
        <source>&amp;Other...</source>
        <translation type="obsolete">&amp;Інше...</translation>
    </message>
    <message>
        <source>&amp;Cascade</source>
        <translation type="obsolete">&amp;Каскадне розміщення</translation>
    </message>
    <message>
        <source>&amp;Tile</source>
        <translation type="obsolete">&amp;Мозаїчне розміщення</translation>
    </message>
    <message>
        <source>&amp;Color</source>
        <translation>&amp;Колір</translation>
    </message>
    <message>
        <source>&amp;Invert</source>
        <translation type="obsolete">&amp;Негатив</translation>
    </message>
    <message>
        <source>&amp;Get Text...</source>
        <translation type="obsolete">Вставити &amp;текст...</translation>
    </message>
    <message>
        <source>&amp;Font</source>
        <translation>&amp;Шрифт</translation>
    </message>
    <message>
        <source>&amp;Size</source>
        <translation>&amp;Розмір</translation>
    </message>
    <message>
        <source>&amp;Effects</source>
        <translation>&amp;Ефекти</translation>
    </message>
    <message>
        <source>&amp;Alignment</source>
        <translation>Ви&amp;рівнювання</translation>
    </message>
    <message>
        <source>&amp;Shade</source>
        <translation>&amp;Тінь</translation>
    </message>
    <message>
        <source>&amp;Tabulators...</source>
        <translation type="obsolete">&amp;Табулятори...</translation>
    </message>
    <message>
        <source>Un&amp;lock</source>
        <translation type="obsolete">&amp;Відімкнути</translation>
    </message>
    <message>
        <source>Show &amp;Images</source>
        <translation type="obsolete">&amp;Показати зображення</translation>
    </message>
    <message>
        <source>Show &amp;Margins</source>
        <translation type="obsolete">Показати &amp;розмітку полів</translation>
    </message>
    <message>
        <source>Show &amp;Frames</source>
        <translation type="obsolete">Показати ра&amp;мки</translation>
    </message>
    <message>
        <source>Show G&amp;uides</source>
        <translation type="obsolete">Показати розм&amp;ітку</translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS Images</source>
        <translation type="obsolete">Ghostscript : ви не можете використовувати EPS зображення</translation>
    </message>
    <message>
        <source>Import &amp;Page(s)...</source>
        <translation type="obsolete">Імпортувати &amp;Сторінку(и)...</translation>
    </message>
    <message>
        <source>Importing Pages...</source>
        <translation>Імпорт Сторінок...</translation>
    </message>
    <message>
        <source>Import Page(s)</source>
        <translation>Імпортувати сторінку(и)</translation>
    </message>
    <message>
        <source>&lt;p&gt;You are trying to import more pages than there are available in the current document counting from the active page.&lt;/p&gt;Choose one of the following:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Create&lt;/b&gt; missing pages&lt;/li&gt;&lt;li&gt;&lt;b&gt;Import&lt;/b&gt; pages until the last page&lt;/li&gt;&lt;li&gt;&lt;b&gt;Cancel&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</source>
        <translation>&lt;p&gt;Ви намагаєтеся імпортувати більше число сторінок, ніж є у цьому документі, рахуючи з активної сторінки.&lt;/p&gt;Виберіть з:&lt;br&gt;&lt;ul&gt;&lt;li&gt;&lt;b&gt;Створити&lt;/b&gt; недостаючі сторінки&lt;/li&gt;&lt;li&gt;&lt;b&gt;Імпортувати&lt;/b&gt; сторінки до останньої сторінки&lt;/li&gt;&lt;li&gt;&lt;b&gt;Відмінити&lt;/b&gt;&lt;/li&gt;&lt;/ul&gt;&lt;br&gt;</translation>
    </message>
    <message>
        <source>Create</source>
        <translation>Створити</translation>
    </message>
    <message>
        <source>Import</source>
        <translation>Імпортувати</translation>
    </message>
    <message>
        <source>Import done</source>
        <translation>Імпортування закінчено</translation>
    </message>
    <message>
        <source>Found nothing to import</source>
        <translation>Не знаходжу, що імпортувати</translation>
    </message>
    <message>
        <source>100%</source>
        <translation type="obsolete">100%</translation>
    </message>
    <message>
        <source>Sn&amp;ap to Grid</source>
        <translation type="obsolete">П&amp;ритягування до сітки</translation>
    </message>
    <message>
        <source>P&amp;references...</source>
        <translation type="obsolete">&amp;Установки...</translation>
    </message>
    <message>
        <source>Getting ICC Profiles</source>
        <translation type="obsolete">Отримання ICC профілів</translation>
    </message>
    <message>
        <source>Manage &amp;Guides...</source>
        <translation type="obsolete">Установка роз&amp;мітки...</translation>
    </message>
    <message>
        <source>&amp;Size:</source>
        <translation>&amp;Розмір:</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Тінь:</translation>
    </message>
    <message>
        <source>Document &amp;Information...</source>
        <translation type="obsolete">Інформація про &amp;документ...</translation>
    </message>
    <message>
        <source>&amp;Undo Delete Object</source>
        <translation type="obsolete">&amp;Відмінити видалення об&apos;єкта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Move</source>
        <translation type="obsolete">Відмінити &amp;переміщення об&apos;єкта</translation>
    </message>
    <message>
        <source>&amp;Undo Object Change</source>
        <translation type="obsolete">Відмінити &amp;зміну об&apos;єкта</translation>
    </message>
    <message>
        <source>&amp;Edit Shape</source>
        <translation type="obsolete">&amp;Редагувати текст</translation>
    </message>
    <message>
        <source>File %1 is not in Scribus format</source>
        <translation type="obsolete">Файл %1 не відповідає формату Scribus</translation>
    </message>
    <message>
        <source>Afrikaans</source>
        <translation>Африкаанс</translation>
    </message>
    <message>
        <source>Font System Initialized</source>
        <translation>Шрифтова система ініціалізована</translation>
    </message>
    <message>
        <source>Portuguese (BR)</source>
        <translation>Португальська (Бр)</translation>
    </message>
    <message>
        <source>There are no suitable Fonts on your System</source>
        <translation type="obsolete">У Вашій системі відсутні відповідні шрифти</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is supported but can not currently be enabled. Make sure you have ICC color profiles installed and that the profile path in the preferences points to where they&apos;re installed.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Керування кольорами підтримується програмою, але не може бути ввімкненим. Впевніться, що Ви маєте набір кольорових профілів ICC та шлях до цих профілів правильно вказаний в установках.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Color management is not supported by this Scribus build (not compiled in).&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Керування кольорами не підтримується цією збіркою Scribus (CMS підтримка не була вбудована при збірці програми з сирців).&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Croatian</source>
        <translation>Хорватська</translation>
    </message>
    <message>
        <source>Initializing Keyboard Shortcuts</source>
        <translation type="unfinished"></translation>
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
        <source>Edit</source>
        <translation type="unfinished">Редагувати</translation>
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
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Level</source>
        <translation type="unfinished">План</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Перемістити на &amp;план</translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;Установки PDF</translation>
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
        <translation type="unfinished">Символ</translation>
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
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Postscript</source>
        <translation type="obsolete">Постскрипт</translation>
    </message>
    <message>
        <source>Some fonts used by this document have been substituted:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> was replaced by: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Some ICC profiles used by this document are not installed:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>(converted)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Cannot write the file: 
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Save As</source>
        <translation type="unfinished">Записати як</translation>
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
        <source>The following programs are missing:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Ghostscript : You cannot use EPS images or Print Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>EPS Files (*.eps);;All Files (*)</source>
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
        <source>Bulgarian</source>
        <translation type="unfinished">Болгарська</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="unfinished">Інформація</translation>
    </message>
    <message>
        <source>is missing!</source>
        <translation type="obsolete">не існує!</translation>
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
        <source>File %1 is not in an acceptable format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>PostScript</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus has detected some errors.
Consider using the Pre-flight Checker to correct them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Abort</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Ignore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">Ім&apos;я:</translation>
    </message>
    <message>
        <source>New Master Page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Scribus detected some errors.
Consider using the Preflight Verifier  to correct them.</source>
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
        <source>The program %1 is already running!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The program %1 is missing!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Convert Page to Master Page</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScribusColorList</name>
    <message>
        <source>Sample</source>
        <translation type="unfinished">Зразок</translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">Колір</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Новий план</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document</source>
        <translation type="unfinished">Документ</translation>
    </message>
    <message>
        <source>Background</source>
        <translation type="unfinished">Фон</translation>
    </message>
</context>
<context>
    <name>ScribusView</name>
    <message>
        <source> %</source>
        <translation>%</translation>
    </message>
    <message>
        <source>Layer</source>
        <translation>План</translation>
    </message>
    <message>
        <source>All</source>
        <translation type="obsolete">Все</translation>
    </message>
    <message>
        <source>pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source>mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source>in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source>p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>Copy of</source>
        <translation type="obsolete">Копія</translation>
    </message>
    <message>
        <source>Copy Here</source>
        <translation type="unfinished">Копіювати сюди</translation>
    </message>
    <message>
        <source>Move Here</source>
        <translation type="unfinished">Перемістити сюди</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="unfinished">Вихід</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation type="unfinished">&amp;Вклеїти</translation>
    </message>
    <message>
        <source>Picture</source>
        <translation type="unfinished">Зображення</translation>
    </message>
    <message>
        <source>File: </source>
        <translation type="unfinished">Файл:</translation>
    </message>
    <message>
        <source>Original PPI: </source>
        <translation type="unfinished">Початкові ТНД: </translation>
    </message>
    <message>
        <source>Actual PPI: </source>
        <translation type="unfinished">Фактичні ТНД:</translation>
    </message>
    <message>
        <source>Linked Text</source>
        <translation type="unfinished">Зв&apos;язаний текст</translation>
    </message>
    <message>
        <source>Text Frame</source>
        <translation type="unfinished">Текстова рамка</translation>
    </message>
    <message>
        <source>Text on a Path</source>
        <translation type="unfinished">Текст на шляху</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation type="unfinished">Абзаци:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation type="unfinished">Слова:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation type="unfinished">Символи:</translation>
    </message>
    <message>
        <source>Print: </source>
        <translation type="unfinished">Друкувати:</translation>
    </message>
    <message>
        <source>Enabled</source>
        <translation type="unfinished">Ввімкнено</translation>
    </message>
    <message>
        <source>Disabled</source>
        <translation type="unfinished">Вимкнено</translation>
    </message>
    <message>
        <source>In&amp;fo</source>
        <translation type="unfinished">&amp;Інформація</translation>
    </message>
    <message>
        <source>Preview Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;PDF Options</source>
        <translation type="unfinished">&amp;Установки PDF</translation>
    </message>
    <message>
        <source>Send to La&amp;yer</source>
        <translation type="unfinished">Перемістити на &amp;план</translation>
    </message>
    <message>
        <source>Le&amp;vel</source>
        <translation type="unfinished">Пла&amp;н</translation>
    </message>
    <message>
        <source>Conve&amp;rt to</source>
        <translation type="unfinished">&amp;Конвертувати в</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
    </message>
    <message>
        <source>Linking Text Frames</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">Застереження</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ви справді бажаєте очистити весь Ваш текст?</translation>
    </message>
    <message>
        <source>Cannot Delete In-Use Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>The item %1 is currently being edited by Story Editor. The delete operation will be cancelled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>You are trying to link to a filled frame, or a frame to itself.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Do you really want to clear all your text?</source>
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
</context>
<context>
    <name>ScribusWin</name>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Document:</source>
        <translation>Документ:</translation>
    </message>
    <message>
        <source>has been changed since the last save.</source>
        <translation>був змінений з часу останнього запису.</translation>
    </message>
    <message>
        <source>&amp;Leave Anyway</source>
        <translation>Вийти не&amp;гайно</translation>
    </message>
    <message>
        <source>C&amp;lose Anyway</source>
        <translation>Зачинити не&amp;гайно</translation>
    </message>
    <message>
        <source>&amp;Save Now</source>
        <translation>Негайно запи&amp;сати</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
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
        <translation type="obsolete">С&amp;ценарії Scribus</translation>
    </message>
    <message>
        <source>&amp;Execute Script...</source>
        <translation type="obsolete">&amp;Виконати сценарій...</translation>
    </message>
    <message>
        <source>Run a Python script from a file.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Запустити сценарій Пітона з файла.</translation>
    </message>
    <message>
        <source>&amp;Load Extension Script...</source>
        <translation type="obsolete">&amp;Завантажити додатковий сценарій...</translation>
    </message>
    <message>
        <source>Load a Python script as an extension. Used for loading macros and for advanced Python scripts that extend the Scribus user interface.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Завантажити сценарій Пітона, як додаток. Використовується для завантаження макросів та для більш складних сценаріїв на Пітоні, які розширюють інтерфейс користувача Scribus.</translation>
    </message>
    <message>
        <source>&amp;Recent Scripts</source>
        <translation type="obsolete">&amp;Недавно виконані сценарії</translation>
    </message>
    <message>
        <source>Show &amp;Console</source>
        <translation type="obsolete">Показати &amp;Консоль</translation>
    </message>
    <message>
        <source>Display an interactive Python console where you can write and run Python programs that use the Scripter tools.</source>
        <comment>scripter</comment>
        <translation type="obsolete">Показати інтерактивну консоль Пітона в якій Ви можете писати та виконувати програми на Пітоні, які використовують інструментарій сценариста.</translation>
    </message>
    <message>
        <source>&amp;About Script...</source>
        <translation type="obsolete">&amp;Про скрипт...</translation>
    </message>
    <message>
        <source>S&amp;cript</source>
        <translation type="obsolete">С&amp;ценарій</translation>
    </message>
    <message>
        <source>Scripter &amp;Settings</source>
        <comment>script menu</comment>
        <translation type="obsolete">&amp;Установки сценариста</translation>
    </message>
    <message>
        <source>Open</source>
        <translation type="obsolete">Відчинити</translation>
    </message>
    <message>
        <source>Python Scripts (*.py);; All Files (*)</source>
        <translation type="obsolete">Сценарії на Пітоні (*.py);; Всі файли (*)</translation>
    </message>
    <message>
        <source>Script error</source>
        <translation>Помилка сценарія</translation>
    </message>
    <message>
        <source>If you are running an official script report it at &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt; please.</source>
        <translation>Якщо ви використовуєте офіційний сценарій то, будь-ласка, повідомляйте про помилки на &lt;a href=&quot;http://bugs.scribus.net&quot;&gt;bugs.scribus.net&lt;/a&gt;.</translation>
    </message>
    <message>
        <source>This message is in your clipboard too. Use Ctrl+V to paste it into bug tracker.</source>
        <translation>Це повідомлення також розміщене в блоці копіювання. Використайте 
Ctrl-V для вставки його в систему обробітку програмних помилок.</translation>
    </message>
    <message>
        <source>Scribus - Script Plugin</source>
        <translation type="obsolete">Scribus - Модуль зі сценарієм</translation>
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
        <translation type="obsolete">Функція &apos;Завантажити сценарій&apos; модуля сценаріїв в даний момент недоступна.
Якщо Ви всього лиш бажаєте запустити звичайний сценарій - використовуйте
функцію &apos;Виконати сценарй...&apos; натомість.

Якщо Ви справді бажаєте завантажити додатковий сценарій на Пітоні чи макрос
Вам необхідно зробити додаткові сценарії доступними в установках сценариста 
в меню Сценарій.

Будь-ласка спочатку прочитайте документацію по додаткових сценаріях.</translation>
    </message>
    <message>
        <source>Hide &amp;Console</source>
        <translation type="obsolete">Сховати &amp;Консоль</translation>
    </message>
    <message>
        <source>About Script</source>
        <translation type="obsolete">Про скрипт</translation>
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
        <source>Python Scripts (*.py)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Setting up the Python plugin failed. Error details were printed to stderr. </source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ScripterPreferences</name>
    <message>
        <source>Scribus - Scripter Preferences</source>
        <translation type="obsolete">Scribus - Установки сценариста</translation>
    </message>
    <message>
        <source>Enable Scripter Extensions</source>
        <translation type="obsolete">Активувати розширення сценариста</translation>
    </message>
    <message>
        <source>Turn on extension scripts and macros</source>
        <translation type="obsolete">Активувати додаткові сценарії та макроси</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;Enabling scripter extensions turns on additional scripter functionality including Python macros and the option of loading a Python script at start-up. Turning on this option unlocks the &lt;tt&gt;Load Extension Script&lt;/tt&gt; item in the Script menu.&lt;/p&gt;
&lt;p&gt;
Only scripts written to be run as extension scripts should be used with &lt;tt&gt;Load Extension Script&lt;/tt&gt; or as start-up scripts. See the scripter documentation for more details.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Активація розширень сценариста вмикає додаткові функціональні можливості сценариста, включаючи макроси на Пітоні та можливість виконання сценарія на Пітоні при запуску програми.  Вмикання цієї установки розблоковує &lt;tt&gt;Завантажити додатковий сценарій&lt;/tt&gt; в меню Сценарії.&lt;/p&gt;
&lt;p&gt;
Лише сценарії написані для виконання як додаткові сценарії мають використовуватися з &lt;tt&gt;Завантажити додатковий сценарій&lt;/tt&gt; або як сценарії виконувані при запуску програми. Зверніться до документації по сценаристу за додатковою інформацією.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use a Startup Script</source>
        <translation type="obsolete">Використовувати сценарій виконуваний при запуску програми</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;If &lt;tt&gt;Use a Startup Script&lt;/tt&gt; is checked, Scribus will load the script file specified here as an extension script at start-up. It is important that the script be written as an extension script, as if not written carefully it can potentially cause problems.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Use a Startup Script&lt;/tt&gt; will be disabled if scripter extensions are off, as extension scripts cannot be loaded without scripter extensions enabled.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Якщо вибрати &lt;tt&gt;Використовувати сценарій виконуваний при запуску програми&lt;/tt&gt; Scribus буде виконувати вказаний сценарій, як додатковий сценарій. Якщо він містить помилки, вони можуть призвести до виникнення проблем.&lt;/p&gt;
&lt;p&gt;&lt;tt&gt;Використовувати сценарій виконуваний при запуску програми&lt;/tt&gt; буде деактивовано, якщо розширення сценариста було вимкнено, тому що додаткові сценарії можуть бути завантажені лише при активних розширеннях сценариста.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Browse...</source>
        <translation type="obsolete">Проглядати...</translation>
    </message>
    <message>
        <source>Browse for a new script file</source>
        <translation type="obsolete">Проглядати директорії в пошуках нового файла сценарія</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Browse for a new script file&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Проглядати директорії в пошуках нового файла сценарія&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>What script file to load at start-up</source>
        <translation type="obsolete">Який сценарій виконувати при запуску програми</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;The file containing the Python script to run as an extension script at start-up.&lt;/p&gt;
&lt;p&gt;Note that when this script is run, Scribus has not completely started up and the workspace does not yet exist.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;Файл з сценарієм на Пітоні, який буде виконано при запуску програми.&lt;/p&gt;
&lt;p&gt;Майте на увазі, що Scribus ще не повністю запущений і робоче середовище не існує під час запуску цього сценарія.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Script File:</source>
        <translation type="obsolete">Файл сценарія:</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Alt+C</source>
        <translation type="obsolete">Alt+C</translation>
    </message>
    <message>
        <source>Close without saving changes</source>
        <translation type="obsolete">Зачинити без збереження змін</translation>
    </message>
    <message>
        <source>&amp;Ok</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation type="obsolete">Alt+O</translation>
    </message>
    <message>
        <source>Save changes and close</source>
        <translation type="obsolete">Записати зміни і зачинити</translation>
    </message>
    <message>
        <source>Advanced Options</source>
        <translation type="obsolete">Додаткові установки</translation>
    </message>
    <message>
        <source>Import All Names at Startup</source>
        <translation type="obsolete">Імпортувати всі імена під час запуску</translation>
    </message>
    <message>
        <source>Run &apos;from scribus import *&apos; in the script console at start-up</source>
        <translation type="obsolete">Виконати &apos;from scribus import *&apos; в консолі сценариста під час запуску програми</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Import All Names at Startup&lt;/tt&gt; is an advanced option. You should probably leave it checked unless you have read the documentation and know what you are doing.&lt;/p&gt;
&lt;p&gt;Unchecking this option will prevent the scripter from running its usual &lt;tt&gt;from scribus import *&lt;/tt&gt; command when it initializes the main interpreter (used for the script console and extension scripts) at start-up.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Імпортувати всі імена під час запуску програми&lt;/tt&gt; є продвинутою установкою Залиште її незмінною, якщо Ви ще не прочитали документацію і не впевнені чого хочете досягти. &lt;/p&gt;
&lt;p&gt;Вимкнення цієї установки приведе до того, що сценарист не зможе виконати звичайної &lt;tt&gt;from scribus import *&lt;/tt&gt; команди при ініціалізації головного інтерпретарора (використовується для консолі сценариста та додаткових сценаріїв) під час запуску програми.&lt;/p&gt;
&lt;p&gt;Ця установка не буде активована до перезапуску Scribus.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Enable Legacy Name Aliases</source>
        <translation type="obsolete">Дозволити старі аліаси назв</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Enable the use of OldStyle function names&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Дозволити використання старого стилю назв функцій&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Enable Legacy Aliases&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;If checked, this option will cause the scripter to create a large number of function and constant name aliases for 1.2.0 script compatibility. It defaults to checked.&lt;/p&gt;
&lt;p&gt;This option does not take effect until Scribus is restarted.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Дозволити старі аліаси назв&lt;/tt&gt; є продвинутою установкою. Вам певно краще її зараз не чіпати.&lt;/p&gt;
&lt;p&gt;Якщо ввімкнена, ця установка прмусить сценарист створити велику кількість аліасів для назв функцій та констант для сумісності з сценаріями версії Scribus 1.2.0. Ця установка ввімкнена по умовчанню.&lt;/p&gt;
&lt;p&gt;Ця установка не буде активована до перезапуску Scribus.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>Use Fake Stdin</source>
        <translation type="obsolete">Використовувати симульований буфер вводу/виводу</translation>
    </message>
    <message>
        <source>&lt;qt&gt;Replace sys.stdin with a fake file to prevent Scribus hanging when a script tries to read from stdin.&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;Замінити sys.stdin симульованим файлом для попередження зависання Scribus при спробі сценарія зчитати щось із stdin.&lt;/qt&gt;</translation>
    </message>
    <message>
        <source>&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Use Fake Stdin&lt;/tt&gt; is an advanced option. You should probably leave it how it is.&lt;/p&gt;
&lt;p&gt;Normally, scribus will provide Python with a fake file object for &lt;tt&gt;sys.stdin&lt;/tt&gt;, so that reads from stdin always return an empty string. If the real &lt;tt&gt;sys.stdin&lt;/tt&gt; is left in place, scripts that try to read from it will block - and in turn block scribus&apos;s execution, making the app appear to hang - until input arrives on stdin. It&apos;s unusual for GUI apps to expect anything on stdin, so mostly users will think scribus has crashed.&lt;/p&gt;
&lt;p&gt;You can disable this option if you want to accept input on stdin. Generally you should use &lt;tt&gt;os.popen&lt;/tt&gt; to make a pipe instead, or use some other input mechanism, but this option is here just in case.&lt;/p&gt;&lt;/qt&gt;</source>
        <translation type="obsolete">&lt;qt&gt;&lt;p&gt;&lt;tt&gt;Використовувати симульований буфер вводу/виводу&lt;/tt&gt; є продвинутою установкою. Вам певно краще її зараз не чіпати.&lt;/p&gt;
&lt;p&gt;Звичайно scribus створює симульований файловий об&apos;єкт для Пітона - &lt;tt&gt;sys.stdin&lt;/tt&gt;, щоб зчитування з stdin завжди повертали пусту строкову величину. Якщо &lt;tt&gt;sys.stdin&lt;/tt&gt; не чіпати, сценарї, що намагаються читати з нього будуть заблоковані і в свою чергу заблокують роботу scribus, що приведе до зависання програми до появи вхідних даних в stdin. Для графічної програми було б незвичайно очікувати що-небуть з stdin, тому для більшості користувачів це виглядатиме як зависання scribus.&lt;/p&gt;
&lt;p&gt;Ви можете вимкнути цю установку, якщо Ви дійсно очікуєте ввід інформації через stdin. В більшості випадків краще звернутися до &lt;tt&gt;os.popen&lt;/tt&gt; для відкриття труби або використати якийсь інший механізм, але ця установка теж залишається доступною на всяк випадок.&lt;/p&gt;&lt;/qt&gt;</translation>
    </message>
</context>
<context>
    <name>ScripterPrefsGui</name>
    <message>
        <source>Scripter Preferences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enable Extension Scripts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Startup Script</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SeList</name>
    <message>
        <source>Show Page Previews</source>
        <translation>Попередній перегляд сторінок</translation>
    </message>
</context>
<context>
    <name>SeView</name>
    <message>
        <source>Show Template Names</source>
        <translation type="obsolete">Показати імена шаблонів</translation>
    </message>
    <message>
        <source>Show Master Page Names</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SearchReplace</name>
    <message>
        <source>Search/Replace</source>
        <translation>Пошук/Заміна</translation>
    </message>
    <message>
        <source>Search for:</source>
        <translation>Шукати:</translation>
    </message>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Paragraph Style</source>
        <translation>Стиль абзацу</translation>
    </message>
    <message>
        <source>Font</source>
        <translation>Шрифт</translation>
    </message>
    <message>
        <source>Font Size</source>
        <translation>Розмір шрифта</translation>
    </message>
    <message>
        <source>Font Effects</source>
        <translation>Ефекти шрифта</translation>
    </message>
    <message>
        <source>Fill Color</source>
        <translation>Колір заповнення</translation>
    </message>
    <message>
        <source>Fill Shade</source>
        <translation>Тінь заповнення</translation>
    </message>
    <message>
        <source>Stroke Color</source>
        <translation>Колір контура</translation>
    </message>
    <message>
        <source>Stroke Shade</source>
        <translation>Тінь контура</translation>
    </message>
    <message>
        <source> pt</source>
        <translation>тчк</translation>
    </message>
    <message>
        <source>None</source>
        <translation>Немає</translation>
    </message>
    <message>
        <source>Replace with:</source>
        <translation>Замінити на:</translation>
    </message>
    <message>
        <source>Search finished</source>
        <translation>Пошук закінчено</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Left</source>
        <translation>Лівий край</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Центр</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Правий край</translation>
    </message>
    <message>
        <source>Block</source>
        <translation>Блок</translation>
    </message>
    <message>
        <source>Forced</source>
        <translation>Вимушене</translation>
    </message>
    <message>
        <source>&amp;Whole Word</source>
        <translation>&amp;Повне слово</translation>
    </message>
    <message>
        <source>&amp;Ignore Case</source>
        <translation>&amp;Ігнорувати регістр</translation>
    </message>
    <message>
        <source>&amp;Search</source>
        <translation>&amp;Пошук</translation>
    </message>
    <message>
        <source>&amp;Replace</source>
        <translation>&amp;Заміна</translation>
    </message>
    <message>
        <source>Replace &amp;All</source>
        <translation>Замінити &amp;всі</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation>&amp;Зачинити</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистити</translation>
    </message>
</context>
<context>
    <name>SeitenPal</name>
    <message>
        <source>Arrange Pages</source>
        <translation>Розташувати сторінки</translation>
    </message>
    <message>
        <source>Available Templates:</source>
        <translation type="obsolete">Доступні шаблони:</translation>
    </message>
    <message>
        <source>Document Pages:</source>
        <translation>Сторінки документу:</translation>
    </message>
    <message>
        <source>Facing Pages</source>
        <translation>Суміжні сторінки</translation>
    </message>
    <message>
        <source>Left Page first</source>
        <translation type="obsolete">Ліва сторінка перша</translation>
    </message>
    <message>
        <source>Drag Pages or Template Pages onto the Trashbin to delete them.</source>
        <translation type="obsolete">Перетягніть сторінки або шаблони в сміттєву корзину для того, щоб видалити їх.</translation>
    </message>
    <message>
        <source>Here are all your Templates, to create a new Page
drag a Template to the Pageview below.</source>
        <translation type="obsolete">Тут всі Ваші шаблони. Щоб створити нову сторінку
перетягніть шаблон на зображення сторінки внизу.</translation>
    </message>
    <message>
        <source>Normal</source>
        <translation>Звичайний</translation>
    </message>
    <message>
        <source>Previews all the pages of your document.</source>
        <translation type="obsolete">Перегляд всіх сторінок документа.</translation>
    </message>
    <message>
        <source>Drag pages or master pages onto the trashbin to delete them</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Previews all the pages of your document</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Here are all your master pages. To create a new page, drag a master page to the page view below</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Available Master Pages:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Left Page First</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SelectFields</name>
    <message>
        <source>Select Fields</source>
        <translation>Вибрати поля</translation>
    </message>
    <message>
        <source>Available Fields</source>
        <translation>Доступні поля</translation>
    </message>
    <message>
        <source>Selected Fields</source>
        <translation>Вибрані поля</translation>
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
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>ShadeButton</name>
    <message>
        <source>Other...</source>
        <translation>Інші...</translation>
    </message>
    <message>
        <source>Shade</source>
        <translation>Тінь</translation>
    </message>
    <message>
        <source>&amp;Shade:</source>
        <translation>&amp;Тінь:</translation>
    </message>
</context>
<context>
    <name>ShadowValues</name>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
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
        <translation type="unfinished">Короткі &amp;Слова...</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Короткі слова</translation>
    </message>
    <message>
        <source>Special plug-in for adding non-breaking spaces before or after so called short words.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SideBar</name>
    <message>
        <source>No Style</source>
        <translation>Стиль відсутній</translation>
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
        <translation>Немає стилю</translation>
    </message>
</context>
<context>
    <name>StilFormate</name>
    <message>
        <source>Edit Styles</source>
        <translation>Редагування стилю</translation>
    </message>
    <message>
        <source>Copy of %1</source>
        <translation>Копія %1</translation>
    </message>
    <message>
        <source>New Style</source>
        <translation>Новий стиль</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>No</source>
        <translation>Ні</translation>
    </message>
    <message>
        <source>Yes</source>
        <translation>Так</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)</source>
        <translation>Документи (*.sla *.sla.gz *.scd *.scd.gz);;Всі файли (*)</translation>
    </message>
    <message>
        <source>Documents (*.sla *.scd);;All Files (*)</source>
        <translation>Документи (*.sla *.scd);;Всі файли (*)</translation>
    </message>
    <message>
        <source>&amp;Append</source>
        <translation type="obsolete">&amp;Добавити</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагувати</translation>
    </message>
    <message>
        <source>D&amp;uplicate</source>
        <translation>&amp;Дублювати</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>&amp;Save</source>
        <translation type="obsolete">&amp;Записати</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Do you really want to delete this Style?</source>
        <translation type="obsolete">Ви справді бажаєте видалити цей стиль?</translation>
    </message>
    <message>
        <source>&amp;Import</source>
        <translation type="unfinished">І&amp;мпортувати</translation>
    </message>
    <message>
        <source>Do you really want to delete this style?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>StoryEditor</name>
    <message>
        <source>Story Editor</source>
        <translation>Редактор тексту</translation>
    </message>
    <message>
        <source>File</source>
        <translation>Файл</translation>
    </message>
    <message>
        <source>Current Paragraph:</source>
        <translation>Активний абзац:</translation>
    </message>
    <message>
        <source>Words: </source>
        <translation>Слова:</translation>
    </message>
    <message>
        <source>Chars: </source>
        <translation>Символи:</translation>
    </message>
    <message>
        <source>Totals:</source>
        <translation>Сумми:</translation>
    </message>
    <message>
        <source>Paragraphs: </source>
        <translation>Абзаци:</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Застереження</translation>
    </message>
    <message>
        <source>Do you really want to lose all your Changes?</source>
        <translation type="obsolete">Ви справді бажаєте втратити всі Ваші зміни?</translation>
    </message>
    <message>
        <source>Do you really want to clear all your Text?</source>
        <translation type="obsolete">Ви справді бажаєте очистити весь Ваш текст?</translation>
    </message>
    <message>
        <source>Open</source>
        <translation>Відчинити</translation>
    </message>
    <message>
        <source>Text Files (*.txt);;All Files(*)</source>
        <translation>Текстові файли (*.txt);;Всі файли(*)</translation>
    </message>
    <message>
        <source>Save as</source>
        <translation>Записати як</translation>
    </message>
    <message>
        <source>Update Text Frame</source>
        <translation>Обновити вміст текстової рамки</translation>
    </message>
    <message>
        <source>Do you want to save your changes?</source>
        <translation>Записати зміни?</translation>
    </message>
    <message>
        <source>Update Text Frame and Exit</source>
        <translation>Поновити текстову рамку та вийти</translation>
    </message>
    <message>
        <source>Exit Without Updating Text Frame</source>
        <translation>Вийти без поновлення текстової рамки</translation>
    </message>
    <message>
        <source>&amp;Insert Special</source>
        <translation type="obsolete">&amp;Вставити символ</translation>
    </message>
    <message>
        <source>&amp;New</source>
        <translation>&amp;Новий</translation>
    </message>
    <message>
        <source>&amp;Reload Text from Frame</source>
        <translation>&amp;Поновити текст з рамки</translation>
    </message>
    <message>
        <source>&amp;Save to File...</source>
        <translation>За&amp;писати в файл...</translation>
    </message>
    <message>
        <source>&amp;Load from File...</source>
        <translation>&amp;Завантажити з файла...</translation>
    </message>
    <message>
        <source>Save &amp;Document</source>
        <translation>Записати &amp;документ</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame and Exit</source>
        <translation>Поно&amp;вити текстову рамку та вийти</translation>
    </message>
    <message>
        <source>&amp;Exit Without Updating Text Frame</source>
        <translation>Вийти &amp;без поновлення текстової рамки</translation>
    </message>
    <message>
        <source>Cu&amp;t</source>
        <translation>Ви&amp;різати</translation>
    </message>
    <message>
        <source>&amp;Copy</source>
        <translation>&amp;Скопіювати</translation>
    </message>
    <message>
        <source>&amp;Paste</source>
        <translation>&amp;Вклеїти</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистити</translation>
    </message>
    <message>
        <source>&amp;Insert Special...</source>
        <translation type="obsolete">&amp;Вставити символ...</translation>
    </message>
    <message>
        <source>&amp;Update Text Frame</source>
        <translation>&amp;Поновити вміст текстової рамки</translation>
    </message>
    <message>
        <source>&amp;File</source>
        <translation>&amp;Файл</translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation>&amp;Редагувати</translation>
    </message>
    <message>
        <source>Select &amp;All</source>
        <translation>Виді&amp;лити все</translation>
    </message>
    <message>
        <source>&amp;Edit Styles...</source>
        <translation>&amp;Редагувати стилі...</translation>
    </message>
    <message>
        <source>Load Text from File</source>
        <translation>Завантажити текст з файла</translation>
    </message>
    <message>
        <source>Save Text to File</source>
        <translation>Записати текст в файл</translation>
    </message>
    <message>
        <source>Reload Text from Frame</source>
        <translation>Поновити текст з рамки</translation>
    </message>
    <message>
        <source>&amp;Search/Replace...</source>
        <translation>&amp;Пошук/Заміна...</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview...</source>
        <translation>&amp;Попередній перегляд шрифтів...</translation>
    </message>
    <message>
        <source>&amp;Background...</source>
        <translation>&amp;Фон...</translation>
    </message>
    <message>
        <source>&amp;Display Font...</source>
        <translation>&amp;Екранний шрифт...</translation>
    </message>
    <message>
        <source>&amp;Settings</source>
        <translation>&amp;Установки</translation>
    </message>
    <message>
        <source>Search/Replace</source>
        <translation>Пошук/Заміна</translation>
    </message>
    <message>
        <source>&amp;Fonts Preview</source>
        <translation type="obsolete">&amp;Попередній перегляд шрифтів</translation>
    </message>
    <message>
        <source>Clear all Text</source>
        <translation type="obsolete">Очистити весь текст</translation>
    </message>
    <message>
        <source>&amp;Smart text selection</source>
        <translation>&amp;Розумне виділення тексту</translation>
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
</context>
<context>
    <name>StrikeValues</name>
    <message>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
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
        <translation>Підкреслення</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation>Капітель</translation>
    </message>
    <message>
        <source>Subscript</source>
        <translation>Нижній індекс</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation>Верхній індекс</translation>
    </message>
    <message>
        <source>Outline Text</source>
        <translation type="obsolete">Силуетний текст</translation>
    </message>
    <message>
        <source>Strike Out</source>
        <translation>Перекреслений текст</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Shadow</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SxwDialog</name>
    <message>
        <source>OO.o Writer Importer Options</source>
        <translation type="obsolete">Установки імпортера документів OO.o  Writer</translation>
    </message>
    <message>
        <source>Update paragraph styles</source>
        <translation type="obsolete">Поновити стилі абзаців</translation>
    </message>
    <message>
        <source>Use document name as a prefix for paragraph styles</source>
        <translation>Використовувати ім&quot;я документа, як префікс для стилів абзаців</translation>
    </message>
    <message>
        <source>Do not ask again</source>
        <translation>Більше на запитувати</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OO.o document and
never ask your confirmation again</source>
        <translation type="obsolete">Чи бажаєте Ви, щоб імпортер завжди 
використовував теперішню установку 
при імпорті OO.o документів і більше не 
питав Вашого підтвердження</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Should importer add the name of the document
on front of the paragraph style name in Scribus</source>
        <translation type="obsolete">Чи потрібно, щоб імпортер використовував ім&quot;я
документа як префікс для імен стилів Scribus</translation>
    </message>
    <message>
        <source>If a paragraph style already exists with the same name as the current
OpenOffice.org document&apos;s paragraph, should the style in Scribus be
edited to match the one being imported, or left untouched</source>
        <translation type="obsolete">Якщо стиль абзацу імпортований з активного абзацу документа 
OpenOffice.org вже існує, стиль в документі Scribus повинен бути
відредагованим для співпадання з імпортованим стилем, або
залишений незмінним</translation>
    </message>
    <message>
        <source>OpenOffice.org Writer Importer Options</source>
        <translation>Установки імпортера документів OpenOffice.org Writer</translation>
    </message>
    <message>
        <source>Should the importer always use currently
set value when importing OpenOffice.org document and
never ask your confirmation again</source>
        <translation type="obsolete">Чи повинет імпортер завжди використовувати
активне значення без підтверждення</translation>
    </message>
    <message>
        <source>Pack paragraph styles</source>
        <translation type="obsolete">Упакувати стилі абзаців</translation>
    </message>
    <message>
        <source>Group paragraph styles by attributes.
Less paragraph styles but controlling them may be hard.
Should be used if it is known that text must not be edited
after importing.</source>
        <translation type="obsolete">Згрупувати стилі абзаців за атрибутами. Це приводить до
зменшення числа стилів абзаців, але викликає труднощі
в їх контролі. Використовуйте, якщо відомо, що текст не
повинен редагуватися після імпортування.</translation>
    </message>
    <message>
        <source>Update Paragraph Styles</source>
        <translation type="obsolete">Поновити стилі абзаців</translation>
    </message>
    <message>
        <source>Enabling this will overwrite existing styles in the current Scribus document</source>
        <translation>Активація цією установки приведе до перезапису існуючих стилів
в активному документі Scribus</translation>
    </message>
    <message>
        <source>Merge Paragraph Styles</source>
        <translation>Об&apos;єднати стилі абзаців</translation>
    </message>
    <message>
        <source>Merge paragraph styles by attributes. This will result in fewer similar paragraph styles, will retain style attributes, even if the original document&apos;s styles are named differently.</source>
        <translation>Об&apos;єднати стилі абзаців по атрибутах. Це приведе до зменшення числа схожих
стилів абзаців і збереже атрибути стилів навіть якщо стилі вихідного документу
мали інші імена.</translation>
    </message>
    <message>
        <source>Prepend the document name to the paragraph style name in Scribus.</source>
        <translation>Приєднати назву документа як префікс до імені стилю абзацу в
Scribus.</translation>
    </message>
    <message>
        <source>Make these settings the default and do not prompt again when importing an OpenOffice.org 1.x document.</source>
        <translation>Зробити ці установки установками по умовчанню і не запитувати знову під
час імпорту документу OASIS OpenDocument версій 1.х.</translation>
    </message>
    <message>
        <source>Overwrite Paragraph Styles</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TOCIndexPrefs</name>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Додати</translation>
    </message>
    <message>
        <source>Alt+A</source>
        <translation type="unfinished">Alt+A</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation type="unfinished">&amp;Видалити</translation>
    </message>
    <message>
        <source>Alt+D</source>
        <translation type="unfinished">Alt+D</translation>
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
        <translation type="unfinished">тнд</translation>
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
        <translation type="unfinished">пікс</translation>
    </message>
    <message>
        <source>Show Guides</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Color:</source>
        <translation type="unfinished">Колір:</translation>
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
        <translation type="unfinished">Показати базову сітку</translation>
    </message>
    <message>
        <source>Baseline Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Baseline &amp;Grid:</source>
        <translation type="unfinished">&amp;Базова сітка:</translation>
    </message>
    <message>
        <source>Baseline &amp;Offset:</source>
        <translation type="unfinished">Зс&amp;ув базової сітки:</translation>
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
        <translation type="unfinished">Відстань між лініями допоміжної сітки</translation>
    </message>
    <message>
        <source>Distance between the major grid lines</source>
        <translation type="unfinished">Відстань між лініями головної сітки</translation>
    </message>
    <message>
        <source>Distance within which an object will snap to your placed guides</source>
        <translation type="unfinished">Зона притягування об&apos;єкта до розмітки</translation>
    </message>
    <message>
        <source>Radius of the area where Scribus will allow you to grab an objects handles</source>
        <translation type="unfinished">Радіус площі захвату вузлів об&apos;єктів</translation>
    </message>
    <message>
        <source>Color of the minor grid lines</source>
        <translation type="unfinished">Колір ліній допоміжної сітки</translation>
    </message>
    <message>
        <source>Color of the major grid lines</source>
        <translation type="unfinished">Колір ліній головної сітки</translation>
    </message>
    <message>
        <source>Color of the guide lines you insert</source>
        <translation type="unfinished">Колір ліній розмітки</translation>
    </message>
    <message>
        <source>Color for the margin lines</source>
        <translation type="unfinished">Колір границь полів сторінки</translation>
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
        <translation>Налаштування табуляторів</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>TabPDFOptions</name>
    <message>
        <source>Export Range</source>
        <translation type="unfinished">Діапазон експортування</translation>
    </message>
    <message>
        <source>&amp;All Pages</source>
        <translation type="unfinished">&amp;Всі сторінки</translation>
    </message>
    <message>
        <source>C&amp;hoose Pages</source>
        <translation type="unfinished">&amp;Виберіть сторінки</translation>
    </message>
    <message>
        <source>&amp;Rotation:</source>
        <translation type="unfinished">&amp;Кут повороту:</translation>
    </message>
    <message>
        <source>File Options</source>
        <translation type="unfinished">Установки файла</translation>
    </message>
    <message>
        <source>Compatibilit&amp;y:</source>
        <translation type="unfinished">&amp;Сумісність:</translation>
    </message>
    <message>
        <source>&amp;Binding:</source>
        <translation type="unfinished">&amp;Перепліт:</translation>
    </message>
    <message>
        <source>Left Margin</source>
        <translation type="unfinished">Ліва сторона</translation>
    </message>
    <message>
        <source>Right Margin</source>
        <translation type="unfinished">Права сторона</translation>
    </message>
    <message>
        <source>Generate &amp;Thumbnails</source>
        <translation type="unfinished">Створити &amp;Мініатюрні зображення</translation>
    </message>
    <message>
        <source>Save &amp;Linked Text Frames as PDF Articles</source>
        <translation type="unfinished">Зберегти &amp;зв&apos;язані текстові рамки, як статті PDF</translation>
    </message>
    <message>
        <source>&amp;Include Bookmarks</source>
        <translation type="unfinished">Включити &amp;закладки</translation>
    </message>
    <message>
        <source>Include Layers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source> dpi</source>
        <translation type="unfinished">тнд</translation>
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
        <translation type="unfinished">Установки зображення</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation type="unfinished">Автоматичний</translation>
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
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Method:</source>
        <translation type="unfinished">&amp;Метод:</translation>
    </message>
    <message>
        <source>&amp;Quality:</source>
        <translation type="unfinished">&amp;Якість:</translation>
    </message>
    <message>
        <source>Maximum</source>
        <translation type="unfinished">Максимальна</translation>
    </message>
    <message>
        <source>High</source>
        <translation type="unfinished">Висока</translation>
    </message>
    <message>
        <source>Medium</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Low</source>
        <translation type="unfinished">Низька</translation>
    </message>
    <message>
        <source>Minimum</source>
        <translation type="unfinished">Мінімальна</translation>
    </message>
    <message>
        <source>Resample Images to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;General</source>
        <translation type="unfinished">&amp;Загальні</translation>
    </message>
    <message>
        <source>&amp;Embed all Fonts</source>
        <translation type="unfinished">&amp;Вкласти всі шрифти</translation>
    </message>
    <message>
        <source>&amp;Subset all Fonts</source>
        <translation type="unfinished">Провести вибірку всіх &amp;шрифтів</translation>
    </message>
    <message>
        <source>Embedding</source>
        <translation type="unfinished">Вкладання шрифтів</translation>
    </message>
    <message>
        <source>Available Fonts:</source>
        <translation type="unfinished">Доступні шрифти:</translation>
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
        <translation type="unfinished">Шрифти для вкладання:</translation>
    </message>
    <message>
        <source>Fonts to subset:</source>
        <translation type="unfinished">Шрифти для вибірки:</translation>
    </message>
    <message>
        <source>&amp;Fonts</source>
        <translation type="unfinished">&amp;Шрифти</translation>
    </message>
    <message>
        <source>Enable &amp;Presentation Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="unfinished">Сторінка</translation>
    </message>
    <message>
        <source>Show Page Pre&amp;views</source>
        <translation type="unfinished">&amp;Попередній перегляд сторінок</translation>
    </message>
    <message>
        <source>Effects</source>
        <translation type="unfinished">Ефекти</translation>
    </message>
    <message>
        <source>&amp;Display Duration:</source>
        <translation type="unfinished">&amp;Тривалість показу:</translation>
    </message>
    <message>
        <source>Effec&amp;t Duration:</source>
        <translation type="unfinished">Тривалість &amp;ефекту:</translation>
    </message>
    <message>
        <source>Effect T&amp;ype:</source>
        <translation type="unfinished">Т&amp;ип ефекту:</translation>
    </message>
    <message>
        <source>&amp;Moving Lines:</source>
        <translation type="unfinished">&amp;Рухливі лінії:</translation>
    </message>
    <message>
        <source>F&amp;rom the:</source>
        <translation type="unfinished">&amp;Від:</translation>
    </message>
    <message>
        <source>D&amp;irection:</source>
        <translation type="unfinished">&amp;Напрямок:</translation>
    </message>
    <message>
        <source> sec</source>
        <translation type="unfinished">сек</translation>
    </message>
    <message>
        <source>No Effect</source>
        <translation type="unfinished">Ніякого ефекту</translation>
    </message>
    <message>
        <source>Blinds</source>
        <translation type="unfinished">Венська фіранка</translation>
    </message>
    <message>
        <source>Box</source>
        <translation type="unfinished">Коробка</translation>
    </message>
    <message>
        <source>Dissolve</source>
        <translation type="unfinished">Розтавання</translation>
    </message>
    <message>
        <source>Glitter</source>
        <translation type="unfinished">Сяяння</translation>
    </message>
    <message>
        <source>Split</source>
        <translation type="unfinished">Розділення</translation>
    </message>
    <message>
        <source>Wipe</source>
        <translation type="unfinished">Витирання</translation>
    </message>
    <message>
        <source>Horizontal</source>
        <translation type="unfinished">Горизонтально</translation>
    </message>
    <message>
        <source>Vertical</source>
        <translation type="unfinished">Вертикально</translation>
    </message>
    <message>
        <source>Inside</source>
        <translation type="unfinished">Зсередини</translation>
    </message>
    <message>
        <source>Outside</source>
        <translation type="unfinished">Ззовні</translation>
    </message>
    <message>
        <source>Left to Right</source>
        <translation type="unfinished">Зліва направо</translation>
    </message>
    <message>
        <source>Top to Bottom</source>
        <translation type="unfinished">Зверху донизу</translation>
    </message>
    <message>
        <source>Bottom to Top</source>
        <translation type="unfinished">Знизу вверх</translation>
    </message>
    <message>
        <source>Right to Left</source>
        <translation type="unfinished">Зправа наліво</translation>
    </message>
    <message>
        <source>Top-left to Bottom-Right</source>
        <translation type="unfinished">Зліва зверху вправо вниз</translation>
    </message>
    <message>
        <source>&amp;Apply Effect on all Pages</source>
        <translation type="unfinished">Застосувати ефект на в&amp;сіх сторінках</translation>
    </message>
    <message>
        <source>E&amp;xtras</source>
        <translation type="unfinished">&amp;Додатки</translation>
    </message>
    <message>
        <source>&amp;Use Encryption</source>
        <translation type="unfinished">Використовувати &amp;шифрування</translation>
    </message>
    <message>
        <source>Passwords</source>
        <translation type="unfinished">Паролі</translation>
    </message>
    <message>
        <source>&amp;User:</source>
        <translation type="unfinished">&amp;Користувач:</translation>
    </message>
    <message>
        <source>&amp;Owner:</source>
        <translation type="unfinished">В&amp;ласник:</translation>
    </message>
    <message>
        <source>Settings</source>
        <translation type="unfinished">Установки</translation>
    </message>
    <message>
        <source>Allow &amp;Printing the Document</source>
        <translation type="unfinished">Дозволити &amp;друк документа</translation>
    </message>
    <message>
        <source>Allow &amp;Changing the Document</source>
        <translation type="unfinished">Дозволити внесення &amp;змін в документ</translation>
    </message>
    <message>
        <source>Allow Cop&amp;ying Text and Graphics</source>
        <translation type="unfinished">Дозволити &amp;копіювання тексту і зображень</translation>
    </message>
    <message>
        <source>Allow Adding &amp;Annotations and Fields</source>
        <translation type="unfinished">Дозволити додавання &amp;аннотацій та полів</translation>
    </message>
    <message>
        <source>S&amp;ecurity</source>
        <translation type="unfinished">&amp;Захист</translation>
    </message>
    <message>
        <source>General</source>
        <translation type="unfinished">Загальні</translation>
    </message>
    <message>
        <source>Output &amp;Intended For:</source>
        <translation type="unfinished">Вихідний документ &amp;призначений для:</translation>
    </message>
    <message>
        <source>Screen / Web</source>
        <translation type="unfinished">Екран комп&apos;ютера/Веб сторінка</translation>
    </message>
    <message>
        <source>Printer</source>
        <translation type="unfinished">Принтер</translation>
    </message>
    <message>
        <source>Grayscale</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Use Custom Rendering Settings</source>
        <translation type="unfinished">Використовувати нестандартні &amp;установки рендерингу</translation>
    </message>
    <message>
        <source>Rendering Settings</source>
        <translation type="unfinished">Установки рендерингу</translation>
    </message>
    <message>
        <source>Fre&amp;quency:</source>
        <translation type="unfinished">&amp;Частота:</translation>
    </message>
    <message>
        <source>&amp;Angle:</source>
        <translation type="unfinished">&amp;Кут:</translation>
    </message>
    <message>
        <source>S&amp;pot Function:</source>
        <translation type="unfinished">&amp;Локальна функція:</translation>
    </message>
    <message>
        <source>Simple Dot</source>
        <translation type="unfinished">Проста точка</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="unfinished">Лінія</translation>
    </message>
    <message>
        <source>Round</source>
        <translation type="unfinished">Круг</translation>
    </message>
    <message>
        <source>Ellipse</source>
        <translation type="unfinished">Елліпс</translation>
    </message>
    <message>
        <source>Solid Colors:</source>
        <translation type="unfinished">Однотонні кольори:</translation>
    </message>
    <message>
        <source>Use ICC Profile</source>
        <translation type="unfinished">Використовувати ICC профіль</translation>
    </message>
    <message>
        <source>Profile:</source>
        <translation type="unfinished">Профіль:</translation>
    </message>
    <message>
        <source>Rendering-Intent:</source>
        <translation type="unfinished">Призначення вихідного зображення:</translation>
    </message>
    <message>
        <source>Perceptual</source>
        <translation type="unfinished">Уявний</translation>
    </message>
    <message>
        <source>Relative Colorimetric</source>
        <translation type="unfinished">Відносна кольорометрія</translation>
    </message>
    <message>
        <source>Saturation</source>
        <translation type="unfinished">Насичення</translation>
    </message>
    <message>
        <source>Absolute Colorimetric</source>
        <translation type="unfinished">Абсолютна кольорометрія</translation>
    </message>
    <message>
        <source>Images:</source>
        <translation type="unfinished">Зображення:</translation>
    </message>
    <message>
        <source>Don&apos;t use embedded ICC profiles</source>
        <translation type="unfinished">Не використовувати внутрішні ICC профілі</translation>
    </message>
    <message>
        <source>C&amp;olor</source>
        <translation type="unfinished">&amp;Колір</translation>
    </message>
    <message>
        <source>PDF/X-3 Output Intent</source>
        <translation type="unfinished">Призначення вихідного документу PDF/X-3</translation>
    </message>
    <message>
        <source>&amp;Info String:</source>
        <translation type="unfinished">&amp;Інформація:</translation>
    </message>
    <message>
        <source>Output &amp;Profile:</source>
        <translation type="unfinished">Профіль &amp;виводу:</translation>
    </message>
    <message>
        <source>Trim Box</source>
        <translation type="unfinished">Обрізати рамку</translation>
    </message>
    <message>
        <source>PDF/X-&amp;3</source>
        <translation type="unfinished">PDF/X-&amp;3</translation>
    </message>
    <message>
        <source>Embed fonts into the PDF. Embedding the fonts
will preserve the layout and appearance of your document.</source>
        <translation type="unfinished">Вбудувати шрифти в PDF. Вбудовування шрифтів.
збереже формат і вигляд документу.</translation>
    </message>
    <message>
        <source>Enables presentation effects when using Adobe&amp;#174; Reader&amp;#174; in full screen mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Show page previews of each page listed above.</source>
        <translation type="unfinished">Показувати попередній перегляд кожної сторінки вказаної вверху.</translation>
    </message>
    <message>
        <source>Length of time the page is shown before the presentation starts on the selected page.</source>
        <translation type="unfinished">Тривалість показу сторінки перед початком прозентації на вибраній сторінці.</translation>
    </message>
    <message>
        <source>Length of time the effect runs.
A shorter time will speed up the effect, a longer one will slow it down.</source>
        <translation type="unfinished">Тривалість ефекту. Менша тривалість пришвидшуе ефект і навпаки.</translation>
    </message>
    <message>
        <source>Type of the display effect.</source>
        <translation type="unfinished">Тип ефекту.</translation>
    </message>
    <message>
        <source>Direction of the effect of moving lines for the split and blind effects.</source>
        <translation type="unfinished">Напрямок руху ліній для &quot;розділений&quot; і &quot;венеціанська штора&quot; ефектів.</translation>
    </message>
    <message>
        <source>Starting position for the box and split effects.</source>
        <translation type="unfinished">Початкова позиція для ефектів &quot;коробка&quot; і &quot;розділений&quot;.</translation>
    </message>
    <message>
        <source>Direction of the glitter or wipe effects.</source>
        <translation type="unfinished">Напрямок руху ефектів &quot;блиск&quot; і &quot;витирання&quot;.</translation>
    </message>
    <message>
        <source>Apply the selected effect to all pages.</source>
        <translation type="unfinished">Застосувати вибраний ефект на всіх сторінках.</translation>
    </message>
    <message>
        <source>Export all pages to PDF</source>
        <translation type="unfinished">Експортувати всі сторінки в PDF</translation>
    </message>
    <message>
        <source>Export a range of pages to PDF</source>
        <translation type="unfinished">Експортувати діапазон сторінок в PDF</translation>
    </message>
    <message>
        <source>Insert a comma separated list of tokens where
a token can be * for all the pages, 1-5 for
a range of pages or a single page number.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the PDF compatibility.
The default is PDF 1.3 which gives the widest compatibility.
Choose PDF 1.4 if your file uses features such as transparency or you require 128 bit encryption.
PDF/X-3 is for exporting the PDF, when you want color managed RGB for commercial printing
 and is selectable when you have activated color management. 
Use only when advised by your printer or in some cases printing to a 4 color digital color laser printer.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Determines the binding of pages in the PDF. Unless you know
you need to change it leave the default choice - Left.</source>
        <translation type="unfinished">Визначає тип переплітання сторінок в PDF. Стандартний варіант - 
переплітання з лівої сторони.</translation>
    </message>
    <message>
        <source>Generates thumbnails of each page in the PDF.
Some viewers can use the thumbnails for navigation.</source>
        <translation type="unfinished">Вкладає зменшені зображення кожної сторінки в PDF.
Деякі програми для перегляду PDF можуть використовувати
ці зображення для навігації.</translation>
    </message>
    <message>
        <source>Generate PDF Articles, which is useful for navigating linked articles in a PDF.</source>
        <translation type="unfinished">Генерувати PDF Статті. Корисні для навігації ссилок на статті в PDF.</translation>
    </message>
    <message>
        <source>Layers in your document are exported to the PDF
Only available if PDF 1.5 is choosen.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Embed the bookmarks you created in your document.
These are useful for navigating long PDF documents.</source>
        <translation type="unfinished">Вбудувати закладки. які Ви зробили в документі, в PDF.
Корисно для навігації довгих PDF документів.</translation>
    </message>
    <message>
        <source>Export resolution of text and vector graphics.
This does not affect the resolution of bitmap images like photos.</source>
        <translation type="unfinished">Розрішення експортованого тексту та зображень.
Не має ніякого ефекту на розрішення растрових зображень,
таких як фотографії.</translation>
    </message>
    <message>
        <source>Enable lossless compression of text and graphics.
Unless you have a reason, leave this checked. This reduces PDF size.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Method of compression to use for images. Automatic allows Scribus to choose the best method. ZIP is lossless and good for images with solid colors. JPEG is better at creating smaller PDF files which have many photos (with slight image quality loss possible). Leave it set to Automatic, unless you have a need for special compression options.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Compression levels: Minimum (25%), Low (50%), Medium (75%), High (85%), Maximum (95%)</source>
        <translation type="unfinished">Ступені стиснення:  Мінімальна (25%), Низка (50%), Середня (75%), Висока (85%), Максимальна (95%)</translation>
    </message>
    <message>
        <source>Re-sample your bitmap images to the selected DPI.
Leaving this unchecked will render them at their native resolution.
This can increase memory usage and slow down export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>DPI (Dots Per Inch) for image export.</source>
        <translation type="unfinished">DPI (Точок на квадратний дюйм) для експорту зображень.</translation>
    </message>
    <message>
        <source>Enable the security features in your exported PDF.
If you selected PDF 1.3, the PDF will be protected by 40 bit encryption.
If you selected PDF 1.4, the PDF will be protected by 128 bit encryption.
Disclaimer: PDF encryption is not as reliable as GPG or PGP encryption and does have some limitations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Choose a master password which enables or disables all the
security features in your exported PDF</source>
        <translation type="unfinished">Виберіть головний пароль для ввімкнення та вимкнення всіх
засобів захисту в експортованому PDF</translation>
    </message>
    <message>
        <source>Choose a password for users to be able to read your PDF.</source>
        <translation type="unfinished">Виберіть пароль для захисту PDF документа від перегляду.</translation>
    </message>
    <message>
        <source>Allow printing of the PDF. If un-checked, printing is prevented. </source>
        <translation type="unfinished">Дозволити друк PDF. Якщо не вибрати цей параметр, то друк буде заборонено.</translation>
    </message>
    <message>
        <source>Allow modifying of the PDF. If un-checked, modifying the PDF is prevented.</source>
        <translation type="unfinished">Дозволити редагування PDF. Якщо цей параметр не вибрати, то редагування PDF буде заборонено.</translation>
    </message>
    <message>
        <source>Allow copying of text or graphics from the PDF. 
If un-checked, text and graphics cannot be copied.</source>
        <translation type="unfinished">Дозволити копіювання тексту і зображень з PDF.
Якщо цей параметр не вибрати, то копіювання 
тексту і зображень буде заборонено.</translation>
    </message>
    <message>
        <source>Allow adding annotations and fields to the PDF. 
If un-checked, editing annotations and fileds is prevented.</source>
        <translation type="unfinished">Дозволити вставку анотацій та полів в PDF.
Якщо цей параметр не вибрати, то редагування
анотацій та полів буде заборонено.</translation>
    </message>
    <message>
        <source>Color model for the output of your PDF.
Choose Screen/Web for PDFs which are used for screen display and for printing on typical inkjets.
Choose Printer when printing to a true 4 color CMYK printer.</source>
        <translation type="unfinished">Кольорова модель для створення PDF.
Виберіть Екран/Веб для PDF документів, призначених для показу на екрані або для друку на типових
струменевих прінтерах.
Виберіть Прінтер для друку на справжніх 4-х кольорових CMYK прінтерах.</translation>
    </message>
    <message>
        <source>This is an advanced setting which is not enabled by default. This should only be enabled
when specifically requested by your printer and they have given you the exact details needed.
Otherwise, your exported PDF may not print properly and is truly not portable across systems.</source>
        <translation type="unfinished">Це додаткова установка, яка не використовується по умовчанню. Вона має бути ввімкнена
лише по вимозі типографії і наявності точних деталей. Інакше експортований PDF може не
друкуватися правильно і не бути придатним для використання на різних платформах.</translation>
    </message>
    <message>
        <source>Embed a color profile for solid colors</source>
        <translation type="unfinished">Вбудувати кольоровий профіль для однотонних кольорів</translation>
    </message>
    <message>
        <source>Color profile for solid colors</source>
        <translation type="unfinished">Кольоровий профіль для однотонних кольорів</translation>
    </message>
    <message>
        <source>Rendering intent for solid colors</source>
        <translation type="unfinished">Схема перерахунку кольорів для однотонних кольорів</translation>
    </message>
    <message>
        <source>Embed a color profile for images</source>
        <translation type="unfinished">Вбудований кольоровий профіль для зображень</translation>
    </message>
    <message>
        <source>Do not use color profiles that are embedded in source images</source>
        <translation type="unfinished">Не використовувати кольорові профілі вбудовані в оригінальні зображення</translation>
    </message>
    <message>
        <source>Color profile for images</source>
        <translation type="unfinished">Кольоровий профіль для зображень</translation>
    </message>
    <message>
        <source>Rendering intent for images</source>
        <translation type="unfinished">Схема перерахунку кольорів для зображень</translation>
    </message>
    <message>
        <source>Output profile for printing. If possible, get some guidance from your printer on profile selection.</source>
        <translation type="unfinished">Профіль для друку. При можливості скористайтеся порадами 
технолога типографії для правильного вибору цього профіля.</translation>
    </message>
    <message>
        <source>Mandatory string for PDF/X-3 or the PDF will fail
PDF/X-3 conformance. We recommend you use the title of the document.</source>
        <translation type="unfinished">Цей запис вимагається форматом PDF/X-3. Без нього PDF не пройде перевірку
на відповідність формату PDF/X-3. Ми рекомендуємо використання заголовку документу.</translation>
    </message>
    <message>
        <source>Distance for bleed from the top of the physical page</source>
        <translation type="unfinished">Розмір поля для перекриття з верхньої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the bottom of the physical page</source>
        <translation type="unfinished">Розмір поля для перекриття з нижньої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the left of the physical page</source>
        <translation type="unfinished">Розмір поля для перекриття з лівої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Distance for bleed from the right of the physical page</source>
        <translation type="unfinished">Розмір поля для перекриття з правої сторони фізичної сторінки</translation>
    </message>
    <message>
        <source>Mirror Page(s) horizontally</source>
        <translation type="unfinished">Віддзеркалити сторінку(и) горизонтально</translation>
    </message>
    <message>
        <source>Mirror Page(s) vertically</source>
        <translation type="unfinished">Віддзеркалити сторінку(и) вертикально</translation>
    </message>
    <message>
        <source>Convert Spot Colors to Process Colors</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TabTools</name>
    <message>
        <source>Font:</source>
        <translation type="unfinished">Шрифт:</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Size:</source>
        <translation type="unfinished">Розмір:</translation>
    </message>
    <message>
        <source>None</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">&amp;Стовпці:</translation>
    </message>
    <message>
        <source>&amp;Gap:</source>
        <translation type="unfinished">&amp;Проміжок:</translation>
    </message>
    <message>
        <source>Woven silk pyjamas exchanged for blue quartz</source>
        <translation type="unfinished">Реве та стогне Дніпр широкий, сердитий вітер завива</translation>
    </message>
    <message>
        <source>&amp;Line Color:</source>
        <translation type="unfinished">Колір &amp;лінії:</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Shading:</source>
        <translation type="unfinished">&amp;Затінення:</translation>
    </message>
    <message>
        <source>&amp;Fill Color:</source>
        <translation type="unfinished">Колір запо&amp;внення:</translation>
    </message>
    <message>
        <source>S&amp;hading:</source>
        <translation type="unfinished">&amp;Затінення:</translation>
    </message>
    <message>
        <source>Line Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Line &amp;Width:</source>
        <translation type="unfinished">Тов&amp;щина лінії:</translation>
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
        <translation type="unfinished">&amp;Вільне масштабування</translation>
    </message>
    <message>
        <source>&amp;Horizontal Scaling:</source>
        <translation type="unfinished">&amp;Горизонтальне масштабування:</translation>
    </message>
    <message>
        <source>&amp;Vertical Scaling:</source>
        <translation type="unfinished">&amp;Вертикальне масштабування:</translation>
    </message>
    <message>
        <source>&amp;Scale Picture to Frame Size</source>
        <translation type="unfinished">Установити масштаб зображення по розміру &amp;рамки</translation>
    </message>
    <message>
        <source>Keep Aspect &amp;Ratio</source>
        <translation type="unfinished">Зберігати &amp;співвідношення сторін</translation>
    </message>
    <message>
        <source>F&amp;ill Color:</source>
        <translation type="unfinished">Колір запо&amp;внення:</translation>
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
        <translation type="unfinished">&amp;Мінімум:</translation>
    </message>
    <message>
        <source>Ma&amp;ximum:</source>
        <translation type="unfinished">Ма&amp;ксимум:</translation>
    </message>
    <message>
        <source>&amp;Stepping:</source>
        <translation type="unfinished">&amp;Крок:</translation>
    </message>
    <message>
        <source>Text Frame Properties</source>
        <translation type="unfinished">Властивості текстової рамки</translation>
    </message>
    <message>
        <source>Picture Frame Properties</source>
        <translation type="unfinished">Властивості рамки зображення</translation>
    </message>
    <message>
        <source>Shape Drawing Properties</source>
        <translation type="unfinished">Властивості рисування геометричних фігур</translation>
    </message>
    <message>
        <source>Magnification Level Defaults</source>
        <translation type="unfinished">Стандартні установки масштабу зображення</translation>
    </message>
    <message>
        <source>Line Drawing Properties</source>
        <translation type="unfinished">Властивості рисування лінії</translation>
    </message>
    <message>
        <source>Polygon Drawing Properties</source>
        <translation type="unfinished">Властивості рисування полігонів</translation>
    </message>
    <message>
        <source>Font for new text frames</source>
        <translation type="unfinished">Шрифт для нових текстових рамок</translation>
    </message>
    <message>
        <source>Size of font for new text frames</source>
        <translation type="unfinished">Розмір шрифта для нових текстових рамок</translation>
    </message>
    <message>
        <source>Color of font</source>
        <translation type="unfinished">Колір шрифта</translation>
    </message>
    <message>
        <source>Number of columns in a text frame</source>
        <translation type="unfinished">Число стовпців в текстовій рамці</translation>
    </message>
    <message>
        <source>Gap between text frame columns</source>
        <translation type="unfinished">Проміжок між стовпцями текстової рамки</translation>
    </message>
    <message>
        <source>Sample of your font</source>
        <translation type="unfinished">Зразок Вашого шрифта</translation>
    </message>
    <message>
        <source>Picture frames allow pictures to scale to any size</source>
        <translation type="unfinished">Рамки для зображень дозволяють масштабування зображень до любого розміру</translation>
    </message>
    <message>
        <source>Horizontal scaling of images</source>
        <translation type="unfinished">Горизонтальне масштабування зображень</translation>
    </message>
    <message>
        <source>Vertical scaling of images</source>
        <translation type="unfinished">Вертикальне масштабування зображень</translation>
    </message>
    <message>
        <source>Keep horizontal and vertical scaling the same</source>
        <translation type="unfinished">Утримувати горизонтальне масштабування рівним вертикальному</translation>
    </message>
    <message>
        <source>Pictures in picture frames are scaled to the size of the frame</source>
        <translation type="unfinished">Зображення у рамках для зображень масштабовані до розміру рамки</translation>
    </message>
    <message>
        <source>Automatically scaled pictures keep their original proportions</source>
        <translation type="unfinished">Автоматично масштабовані зображення зберігають відношення сторін</translation>
    </message>
    <message>
        <source>Fill color of picture frames</source>
        <translation type="unfinished">Колір заповнення рамок зображень</translation>
    </message>
    <message>
        <source>Saturation of color of fill</source>
        <translation type="unfinished">Насиченість кольору заповнення</translation>
    </message>
    <message>
        <source>Line color of shapes</source>
        <translation type="unfinished">Колір ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Saturation of color of lines</source>
        <translation type="unfinished">Насиченість кольору ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Fill color of shapes</source>
        <translation type="unfinished">Колір заповнення геометричних фігур</translation>
    </message>
    <message>
        <source>Line style of shapes</source>
        <translation type="unfinished">Стиль ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Line width of shapes</source>
        <translation type="unfinished">Ширина ліній геометричних фігур</translation>
    </message>
    <message>
        <source>Minimum magnification allowed</source>
        <translation type="unfinished">Мінімально дозволений масштаб зображення</translation>
    </message>
    <message>
        <source>Maximum magnification allowed</source>
        <translation type="unfinished">Максимально дозволений масштаб зображення</translation>
    </message>
    <message>
        <source>Change in magnification for each zoom operation</source>
        <translation type="unfinished">Зміна масштабу зображення при кожному кроці масштабування</translation>
    </message>
    <message>
        <source>Color of lines</source>
        <translation type="unfinished">Колір ліній</translation>
    </message>
    <message>
        <source>Saturation of color</source>
        <translation type="unfinished">Насиченість кольору</translation>
    </message>
    <message>
        <source>Style of lines</source>
        <translation type="unfinished">Стиль ліній</translation>
    </message>
    <message>
        <source>Width of lines</source>
        <translation type="unfinished">Товщина ліній</translation>
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
        <translation type="unfinished">Нестандартний</translation>
    </message>
</context>
<context>
    <name>TabTypograpy</name>
    <message>
        <source>Subscript</source>
        <translation type="unfinished">Нижній індекс</translation>
    </message>
    <message>
        <source> %</source>
        <translation type="unfinished">%</translation>
    </message>
    <message>
        <source>&amp;Displacement:</source>
        <translation type="unfinished">Змі&amp;щення:</translation>
    </message>
    <message>
        <source>&amp;Scaling:</source>
        <translation type="unfinished">&amp;Масштабування:</translation>
    </message>
    <message>
        <source>Superscript</source>
        <translation type="unfinished">Верхній індекс</translation>
    </message>
    <message>
        <source>D&amp;isplacement:</source>
        <translation type="unfinished">Змі&amp;щення:</translation>
    </message>
    <message>
        <source>S&amp;caling:</source>
        <translation type="unfinished">&amp;Масштабування:</translation>
    </message>
    <message>
        <source>Underline</source>
        <translation type="unfinished"></translation>
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
        <translation type="unfinished">Товщина лінії:</translation>
    </message>
    <message>
        <source>Strikethru</source>
        <translation type="unfinished">Перекреслення</translation>
    </message>
    <message>
        <source>Small Caps</source>
        <translation type="unfinished">Капітель</translation>
    </message>
    <message>
        <source>Sc&amp;aling:</source>
        <translation type="unfinished">&amp;Масштабування:</translation>
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
        <translation type="unfinished">Зміщення над основою шрифта на лінії</translation>
    </message>
    <message>
        <source>Relative size of the superscript compared to the normal font</source>
        <translation type="unfinished">Відносний розмір верхнього індексу у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Displacement below the baseline of the normal font on a line</source>
        <translation type="unfinished">Зміщення під основою шрифта на лінії</translation>
    </message>
    <message>
        <source>Relative size of the subscript compared to the normal font</source>
        <translation type="unfinished">Відносний розмір нижнього індексу у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Relative size of the small caps font compared to the normal font</source>
        <translation type="unfinished">Відносний розмір капітелі у порівнянні з звичайним шрифтом</translation>
    </message>
    <message>
        <source>Percentage increase over the font size for the line spacing</source>
        <translation type="unfinished">Збільшення інтервалу між рядками у відсотках розміру шрифта</translation>
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
        <translation>Лівий край</translation>
    </message>
    <message>
        <source>Right</source>
        <translation>Правий край</translation>
    </message>
    <message>
        <source>Full Stop</source>
        <translation>Повна зупинка</translation>
    </message>
    <message>
        <source>Comma</source>
        <translation>Кома</translation>
    </message>
    <message>
        <source>Center</source>
        <translation>Центр</translation>
    </message>
    <message>
        <source>Delete All</source>
        <translation>Видалити всі</translation>
    </message>
    <message>
        <source>Indentation for first line of the paragraph</source>
        <translation>Відступ для першого рядка абзаца</translation>
    </message>
    <message>
        <source>Indentation from the left for the whole paragraph</source>
        <translation>Лівий відступ для всього абзаца</translation>
    </message>
    <message>
        <source>Delete all Tabulators</source>
        <translation>Видалити всі табулятори</translation>
    </message>
    <message>
        <source> pt</source>
        <translation type="obsolete">тчк</translation>
    </message>
    <message>
        <source> mm</source>
        <translation type="obsolete">мм</translation>
    </message>
    <message>
        <source> in</source>
        <translation type="obsolete">д</translation>
    </message>
    <message>
        <source> p</source>
        <translation type="obsolete">п</translation>
    </message>
    <message>
        <source>&amp;Position:</source>
        <translation>По&amp;зиція:</translation>
    </message>
    <message>
        <source>First &amp;Line:</source>
        <translation type="obsolete">Перший &amp;рядок:</translation>
    </message>
    <message>
        <source>Ind&amp;ent:</source>
        <translation type="obsolete">В&amp;ідступ:</translation>
    </message>
    <message>
        <source>Left Ind&amp;ent:</source>
        <translation type="obsolete">Лівий В&amp;ідступ:</translation>
    </message>
    <message>
        <source>None</source>
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
        <translation type="unfinished">Нестандартний</translation>
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
        <translation>Об&apos;єкт</translation>
    </message>
    <message>
        <source>Element</source>
        <translation>Елемент</translation>
    </message>
    <message>
        <source>Type</source>
        <translation type="obsolete">Тип</translation>
    </message>
    <message>
        <source>Information</source>
        <translation type="obsolete">Інформація</translation>
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
        <translation type="obsolete">Шрифт:</translation>
    </message>
    <message>
        <source>Image</source>
        <translation type="obsolete">Зображення</translation>
    </message>
    <message>
        <source>Text</source>
        <translation type="obsolete">Текст</translation>
    </message>
    <message>
        <source>Line</source>
        <translation type="obsolete">Лінія</translation>
    </message>
    <message>
        <source>Polygon</source>
        <translation type="obsolete">Полігон</translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="obsolete">Багатосегментна лінія</translation>
    </message>
    <message>
        <source>PathText</source>
        <translation type="obsolete">Текст на шляху</translation>
    </message>
    <message>
        <source>Page</source>
        <translation type="obsolete">Сторінка</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="obsolete">Застереження</translation>
    </message>
    <message>
        <source>Name &quot;%1&quot; isn&apos;t unique.
Please choose another.</source>
        <translation type="obsolete">Ім&apos;я &quot;%1&quot; вже використане.
Будь-ласка виберіть інше.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
    </message>
    <message>
        <source>Group </source>
        <translation>Група</translation>
    </message>
    <message>
        <source>Free Objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Page </source>
        <translation type="unfinished">Сторінка</translation>
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
        <translation type="unfinished">%</translation>
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
        <translation type="unfinished">Згрупувати</translation>
    </message>
    <message>
        <source>Selection/Group</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Create</source>
        <translation type="unfinished">Створити</translation>
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
        <translation type="unfinished">Вихід</translation>
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
        <translation type="unfinished">Замкнути</translation>
    </message>
    <message>
        <source>Unlock</source>
        <translation type="unfinished">Відімкнути</translation>
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
        <translation type="unfinished">Видалити</translation>
    </message>
    <message>
        <source>Rename</source>
        <translation type="unfinished">Перейменувати</translation>
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
        <translation type="unfinished">Вклеїти</translation>
    </message>
    <message>
        <source>Cut</source>
        <translation type="unfinished">Вирізати</translation>
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
        <translation type="unfinished">Полігон</translation>
    </message>
    <message>
        <source>Bezier curve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Polyline</source>
        <translation type="unfinished">Багатосегментна лінія</translation>
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
        <translation type="unfinished">&amp;Відмінити</translation>
    </message>
    <message>
        <source>&amp;Redo</source>
        <translation type="unfinished">&amp;Повторити</translation>
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
    <name>ValueDialog</name>
    <message>
        <source>Insert value</source>
        <translation>Введіть значення</translation>
    </message>
    <message>
        <source>Enter a value then press OK.</source>
        <translation>Введіть значення і натисніть Гаразд.</translation>
    </message>
    <message>
        <source>Enter a value then press OK</source>
        <translation>Введіть значення і натисніть Гаразд</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <source>Send your value to the script</source>
        <translation>Передати ваше значення сценарію</translation>
    </message>
</context>
<context>
    <name>VlnaDialog</name>
    <message>
        <source>Short Words</source>
        <comment>short words plugin</comment>
        <translation>Короткі слова</translation>
    </message>
    <message>
        <source>Apply unbreakable space on:</source>
        <comment>short words plugin</comment>
        <translation>Вставити нерозбиваємий інтервал в:</translation>
    </message>
    <message>
        <source>&amp;Selected frames</source>
        <comment>short words plugin</comment>
        <translation>&amp;Вибрані рамки</translation>
    </message>
    <message>
        <source>Active &amp;page</source>
        <comment>short words plugin</comment>
        <translation>Активна &amp;сторінка</translation>
    </message>
    <message>
        <source>&amp;All items</source>
        <comment>short words plugin</comment>
        <translation>Всі &amp;Об&apos;єкти</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>&amp;Info and
Languages</source>
        <comment>short words plugin</comment>
        <translation type="obsolete">&amp;Інформація та мови</translation>
    </message>
    <message>
        <source>Replace defaults by user config</source>
        <comment>short words plugin</comment>
        <translation>Замінити значення по умочанню конфігурацією користувача</translation>
    </message>
    <message>
        <source>When the user config file exists 
(%1)
you can choose if you want to append your config
to the global configuration by unchecked button.

You can replace predefined values by yours
with checked button too.</source>
        <comment>short words plugin</comment>
        <translation>Якщо файл з конфігурацією користувача існує
(%1)
Ви можете зробити вибір щодо добавлення вашої 
конфігурації до глобальної конфігурації деактивацією
цієї установки.

Ви можете замістити значення по умовчанню Вашими
власними при натиснутій кнопці.</translation>
    </message>
    <message>
        <source>Only selected frames processed.</source>
        <comment>short words plugin</comment>
        <translation>Обробляються лише вибрані рамки.</translation>
    </message>
    <message>
        <source>Only actual page processed.</source>
        <comment>short words plugin</comment>
        <translation>Обробляється лише активна сторінка.</translation>
    </message>
    <message>
        <source>All items in document processed.</source>
        <comment>short words plugin</comment>
        <translation>Обробляються всі об&apos;єкти в документі.</translation>
    </message>
    <message>
        <source>Short Words for Scribus</source>
        <comment>short words plugin</comment>
        <translation>Короткі Слова для Scribus</translation>
    </message>
    <message>
        <source>Available in the following languages</source>
        <comment>short words plugin</comment>
        <translation>Доступні для наступних мов</translation>
    </message>
    <message>
        <source>About Short Words</source>
        <comment>short words plugin</comment>
        <translation>Про Короткі Слова</translation>
    </message>
    <message>
        <source>Edit &amp;system configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Edit &amp;user configuration...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>S&amp;etup editor...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Info and Languages...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>&amp;Help</source>
        <translation type="unfinished">До&amp;помога</translation>
    </message>
    <message>
        <source>Short Words</source>
        <translation type="unfinished">Короткі слова</translation>
    </message>
    <message>
        <source>You are starting to edit read-only file.
%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Application &apos;%1&apos; error. Cannot be started.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Short Words setup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Enter name of the plain text editor executable:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>WerkToolB</name>
    <message>
        <source>Tools</source>
        <translation>Стандартні інструменти</translation>
    </message>
    <message>
        <source>Select Items</source>
        <translation type="obsolete">Вибрати об&apos;єкт</translation>
    </message>
    <message>
        <source>Insert Text Frame</source>
        <translation type="obsolete">Вставити текстову рамку</translation>
    </message>
    <message>
        <source>Insert Picture</source>
        <translation type="obsolete">Вставити рамку для зображення</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Властивості...</translation>
    </message>
    <message>
        <source>Insert Polygons</source>
        <translation type="obsolete">Вставити полігон</translation>
    </message>
    <message>
        <source>Insert Lines</source>
        <translation type="obsolete">Вставити лінію</translation>
    </message>
    <message>
        <source>Insert Bezier Curves</source>
        <translation type="obsolete">Вставити криву Безьє</translation>
    </message>
    <message>
        <source>Insert Freehand Line</source>
        <translation type="obsolete">Вставити вільно рисовану лінію</translation>
    </message>
    <message>
        <source>Rotate Item</source>
        <translation type="obsolete">Повернути об&apos;єкт</translation>
    </message>
    <message>
        <source>Edit Contents of Frame</source>
        <translation type="obsolete">Редагувати зміст рамки</translation>
    </message>
    <message>
        <source>Link Text Frames</source>
        <translation type="obsolete">Установити зв&apos;язок між текстовими рамками</translation>
    </message>
    <message>
        <source>Unlink Text Frames</source>
        <translation type="obsolete">Розірвати зв&apos;язок між текстовими рамками</translation>
    </message>
    <message>
        <source>Zoom in or out</source>
        <translation type="obsolete">Змінити масштаб зображення</translation>
    </message>
    <message>
        <source>Edit the text with the Story Editor</source>
        <translation type="obsolete">Редагувати текст вбудованим редактором</translation>
    </message>
    <message>
        <source>Draw various Shapes</source>
        <translation type="obsolete">Вставити геометричну фігуру</translation>
    </message>
    <message>
        <source>Insert Table</source>
        <translation type="obsolete">Вставити таблицю</translation>
    </message>
    <message>
        <source>Do measurements</source>
        <translation type="obsolete">Виміряти</translation>
    </message>
</context>
<context>
    <name>WerkToolBP</name>
    <message>
        <source>Text</source>
        <translation>Текст</translation>
    </message>
    <message>
        <source>Link</source>
        <translation>Ссилка</translation>
    </message>
    <message>
        <source>Button</source>
        <translation>Кнопка</translation>
    </message>
    <message>
        <source>Text Field</source>
        <translation>Текстове поле</translation>
    </message>
    <message>
        <source>Check Box</source>
        <translation>Вибіркове поле</translation>
    </message>
    <message>
        <source>Combo Box</source>
        <translation>Комбінований випадаючий список</translation>
    </message>
    <message>
        <source>List Box</source>
        <translation>Випадаючий список</translation>
    </message>
    <message>
        <source>PDF Tools</source>
        <translation>PDF Інструменти</translation>
    </message>
    <message>
        <source>Insert PDF Fields</source>
        <translation>Вставити PDF поля</translation>
    </message>
    <message>
        <source>Insert PDF Annotations</source>
        <translation>Вставити PDF аннотації</translation>
    </message>
</context>
<context>
    <name>ZAuswahl</name>
    <message>
        <source>Select Character:</source>
        <translation type="obsolete">Вибрати символ:</translation>
    </message>
    <message>
        <source>Insert the characters at the cursor in the text</source>
        <translation type="obsolete">Вставити символи в текст в місці знаходження курсора</translation>
    </message>
    <message>
        <source>Delete the current selection(s).</source>
        <translation type="obsolete">Видалити активну вибірку.</translation>
    </message>
    <message>
        <source>Close this dialog and return to text editing.</source>
        <translation type="obsolete">Зачинити цей діалог та повернутися до редагування тексту.</translation>
    </message>
    <message>
        <source>&amp;Insert</source>
        <translation type="obsolete">&amp;Вставити</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation type="obsolete">О&amp;чистити</translation>
    </message>
    <message>
        <source>&amp;Close</source>
        <translation type="obsolete">&amp;Зачинити</translation>
    </message>
</context>
<context>
    <name>gtFileDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Вибрати імпортер</translation>
    </message>
    <message>
        <source>Automatic</source>
        <translation>Автоматичний</translation>
    </message>
    <message>
        <source>Get text only</source>
        <translation type="obsolete">Імпортувати лише текст</translation>
    </message>
    <message>
        <source>Import text without any formatting</source>
        <translation>Імпортувати текст без ніякого форматування</translation>
    </message>
    <message>
        <source>Importer:</source>
        <translation>Імпортер:</translation>
    </message>
    <message>
        <source>Encoding:</source>
        <translation>Кодування:</translation>
    </message>
    <message>
        <source>Import Text Only</source>
        <translation>Імпортувати лише текст</translation>
    </message>
</context>
<context>
    <name>gtImporterDialog</name>
    <message>
        <source>Choose the importer to use</source>
        <translation>Вибрати імпортер для використання</translation>
    </message>
    <message>
        <source></source>
        <translation></translation>
    </message>
    <message>
        <source>Remember association</source>
        <translation>Пам&quot;ятати ассоціацію</translation>
    </message>
    <message>
        <source>Remember the file extension - importer association
and do not ask again to select an importer for
files of this type.</source>
        <translation type="obsolete">Пам&quot;ятати ассоціацію між розширенням файла та
певним імпортером і більше не запитувати про
вибір імпортера для файлів цього типу.</translation>
    </message>
    <message>
        <source>OK</source>
        <translation type="obsolete">Гаразд</translation>
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
        <translation>Новий з шаблона</translation>
    </message>
    <message>
        <source>All</source>
        <translation>Всі</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ім&apos;я</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Розмір сторінки</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Кольори</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Опис</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Використання</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Автор</translation>
    </message>
    <message>
        <source>Created with</source>
        <translation>Створено</translation>
    </message>
    <message>
        <source>&amp;Remove</source>
        <translation>Ви&amp;далити</translation>
    </message>
    <message>
        <source>&amp;Open</source>
        <translation>&amp;Відчинити</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
    <message>
        <source>Downloading Templates</source>
        <translation>Зкачати шаблони</translation>
    </message>
    <message>
        <source>Installing Templates</source>
        <translation>Установка шаблонів</translation>
    </message>
    <message>
        <source>Extract the package to the template directory ~/.scribus/templates for the current user or PREFIX/share/scribus/templates for all users in the system.</source>
        <translation>Розпакуйте архів у директорію для шаблонів користувача (~/.scribus/templates) або в PREFIX/share/scribus/templates для доступу всіх користувачів системи.</translation>
    </message>
    <message>
        <source>Preparing a template</source>
        <translation>Приготування шаблона</translation>
    </message>
    <message>
        <source>Removing a template</source>
        <translation>Видалення шаблона</translation>
    </message>
    <message>
        <source>Translating template.xml</source>
        <translation>Переклад template.xml</translation>
    </message>
    <message>
        <source>Document templates can be found at http://www.scribus.net/ in the Downloads section.</source>
        <translation>Шаблони документів можна знайти на  http://www.scribus.net/ у розділі матеріалів для зкачування.</translation>
    </message>
    <message>
        <source>Make sure images and fonts you use can be used freely. If fonts cannot be shared do not collect them when saving as a template.</source>
        <translation>Впевніться, що зображення та шрифти, використані в шаблоні, дозволяється розповсюджувати. Якщо існують обмеження на їх розповсюдження, то не використовуйте їх &quot;збірку&quot; в документ при запису шаблона.</translation>
    </message>
    <message>
        <source>The template creator should also make sure that the Installing Templates section above applies to their templates as well. This means a user should be able to download a template package and be able to extract them to the template directory and start using them.</source>
        <translation>Автор шаблона повинен впевнитися, що користувач зможе звантажити пакет з його шаблоном і інсталювати його, розпакувавши шаблон у відповідну директорію.</translation>
    </message>
    <message>
        <source>Removing a template from the New From Template dialog will only remove the entry from the template.xml, it will not delete the document files. A popup menu with remove is only shown if you have write access to the template.xml file.</source>
        <translation>Видалення шаблона з діалога &quot;Новий документ з шаблона&quot; лише видалить відповідний пункт з template.xml. Файли не буде видалено.  Діалог видалення активується лише при наявності прав редагування файла template.xml.</translation>
    </message>
    <message>
        <source>Copy an existing template.xml to a file called template.lang_COUNTRY.xml (use the same lang code that is present in the qm file for your language), for example template.fi.xml for Finnish language template.xml. The copy must be located in the same directory as the original template.xml so Scribus can load it.</source>
        <translation>Скопіюйте існуючий файл template.xml у файл template.lang_COUNTRY.xml, де COUNTRY - це код країни який відповідає коду використаному в .qm файлі для Вашої мови. Наприклад &quot;fi&quot; для фінської мови. Ця копія має бути розташована у тій же директорії, що й template.xml для того, щоб Scribus міг її завантажити.</translation>
    </message>
    <message>
        <source>Date</source>
        <translation>Дата</translation>
    </message>
</context>
<context>
    <name>satdialog</name>
    <message>
        <source>Save as Template</source>
        <translation>Записати як шаблон</translation>
    </message>
    <message>
        <source>Name</source>
        <translation>Ім&apos;я</translation>
    </message>
    <message>
        <source>Category</source>
        <translation>Категорія</translation>
    </message>
    <message>
        <source>Page Size</source>
        <translation>Розмір сторінки</translation>
    </message>
    <message>
        <source>Colors</source>
        <translation>Кольори</translation>
    </message>
    <message>
        <source>Description</source>
        <translation>Опис</translation>
    </message>
    <message>
        <source>Usage</source>
        <translation>Використання</translation>
    </message>
    <message>
        <source>Author</source>
        <translation>Автор</translation>
    </message>
    <message>
        <source>Email</source>
        <translation>Адреса електронної пошти</translation>
    </message>
    <message>
        <source>More Details</source>
        <translation>Більш детально</translation>
    </message>
    <message>
        <source>OK</source>
        <translation>Гаразд</translation>
    </message>
    <message>
        <source>Less Details</source>
        <translation>Менш детально</translation>
    </message>
    <message>
        <source>Legal</source>
        <translation>Юридичний</translation>
    </message>
    <message>
        <source>Letter</source>
        <translation>Лист</translation>
    </message>
    <message>
        <source>Tabloid</source>
        <translation>Таблоїд</translation>
    </message>
    <message>
        <source>landscape</source>
        <translation>Горизонтально</translation>
    </message>
    <message>
        <source>portrait</source>
        <translation>Вертикально</translation>
    </message>
    <message>
        <source>custom</source>
        <translation>Установлено користувачем</translation>
    </message>
</context>
<context>
    <name>tfDia</name>
    <message>
        <source>Create filter</source>
        <translation>Створити фільтр</translation>
    </message>
    <message>
        <source>C&amp;lear</source>
        <translation>О&amp;чистити</translation>
    </message>
    <message>
        <source>&amp;Delete</source>
        <translation>&amp;Видалити</translation>
    </message>
    <message>
        <source>Choose a previously saved filter</source>
        <translation>Вибрати попередньо записаний фільтр</translation>
    </message>
    <message>
        <source>Give a name to this filter for saving</source>
        <translation>Дайте ім&quot;я цьому фільтру для його запису</translation>
    </message>
    <message>
        <source>Give a name for saving</source>
        <translation>Дайте ім&quot;я для запису</translation>
    </message>
    <message>
        <source>&amp;OK</source>
        <translation type="obsolete">&amp;Гаразд</translation>
    </message>
    <message>
        <source>&amp;Cancel</source>
        <translation type="obsolete">В&amp;ихід</translation>
    </message>
</context>
<context>
    <name>tfFilter</name>
    <message>
        <source>Disable or enable this filter row</source>
        <translation>Активувати або деактивувати цей рядок фільтрів</translation>
    </message>
    <message>
        <source>Remove this filter row</source>
        <translation>Видалити цей рядок фільтрів</translation>
    </message>
    <message>
        <source>Add a new filter row</source>
        <translation>Додати новий рядок фільтрів</translation>
    </message>
    <message>
        <source>to</source>
        <translation>до</translation>
    </message>
    <message>
        <source>and</source>
        <translation>і</translation>
    </message>
    <message>
        <source>remove match</source>
        <translation>видалити співпадання</translation>
    </message>
    <message>
        <source>do not remove match</source>
        <translation>не видаляти співпадання</translation>
    </message>
    <message>
        <source>words</source>
        <translation>слова</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Видалити</translation>
    </message>
    <message>
        <source>Replace</source>
        <translation>Замінити</translation>
    </message>
    <message>
        <source>Apply</source>
        <translation>Застосувати</translation>
    </message>
    <message>
        <source>Value at the left is a regular expression</source>
        <translation>Значення зліва - це регулярний вираз</translation>
    </message>
    <message>
        <source>with</source>
        <translation>з</translation>
    </message>
    <message>
        <source>paragraph style</source>
        <translation>стиль абзаца</translation>
    </message>
    <message>
        <source>all instances of</source>
        <translation>всі приклади</translation>
    </message>
    <message>
        <source>all paragraphs</source>
        <translation>всі абзаци</translation>
    </message>
    <message>
        <source>paragraphs starting with</source>
        <translation>абзаци починаються з</translation>
    </message>
    <message>
        <source>paragraphs with less than</source>
        <translation>абзаци з менш ніж</translation>
    </message>
    <message>
        <source>paragraphs with more than</source>
        <translation>абзаци з більш ніж</translation>
    </message>
</context>
</TS>
