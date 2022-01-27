.TH "scribus" "1" "Updated 2004-12-16" "" ""
.SH "NAZWA"
scribus \- program do sk³adu publikacji w trybie WYSIWYG dla ¶rodowiska X11 (wolne oprogramowanie na licencji GNU GPL)
.SH "SK£ADNIA"
scribus [\-h|\-\-help] [\-v|\-\-version] [\-l|\-\-lang jêzyk] [\-la|\-\-langs\-available] [\-f|\-\-file|\-\-] [nazwa pliku]
.SH "OPIS"
Scribus jest programem do sk³adu publikacji bêd±cym wolnym oprogramowaniem. Umo¿liwia edycjê w trybie WYSIWYG, doskona³y eksport do formatu PDF i szeroki wybór opcji importu i eksportu.

Wyczerpuj±c± dokumentacjê znale¼æ mo¿na na stronie dokumentacyjnej Scribusa
.I http://docs.scribus.net/
lub w postaci plików pomocy dostêpnych w programie za po¶rednictwem opcji w menu \'Pomoc\'.

Niniejszy podrêcznik systemowy zawiera jedynie skrótowy opis niektórych aspektów zastosowania programu. Jego g³ównym zadaniem jest umo¿liwienie u¿ytkownikowi szybkiego i ³atwego znalezienia pe³nej dokumentacji.
.SH "OPCJE"
Wszystkie opcje programu zawarte s± w instrukcji obs³ugi, któr± wywo³aæ mo¿na poleceniem:
.B scribus \-\-help

.TP 
.B \-l, \-\-lang xx
Nadpisuje locale systemowe i startuje Scribusa w jêzyku xx. Kod jêzyka odpowiada kodom w standardzie POSIX, u¿ywanym w zmiennych ¶rodowiskowych LANG i LC_ALL. Na przyk³ad jêzyk polski mo¿na wybraæ podaj±c 'pl' lub 'pl_PL', jêzyk angielski u¿ywaj±c 'en' (generyczny angielski), 'en_GB' (brytyjski angielski) lub 'en_US' (amerykañski angielski). W podobny sposób mo¿na wybraæ inne jêzyki, np. niemiecki u¿ywaj±c kodów 'de' lub 'de_DE'.
.TP 
.B \-la, \-\-langs\-available
Wy¶wietla listê jêzyków, w których dostêpne jest t³umaczenie interfejsu. Aby wybraæ dany jêzyk, wystartuj Scribusa poleceniem 'scribus \-l xx', w którym xx oznacza kod jêzyka, lub zmieñ odpowiednie zmienne ¶rodowiskowe w sposób opisany poni¿ej.
.TP 
.B \-v, \-\-version
Wy¶wietla numer wersji Scribusa.
.TP 
.B \-f, \-\-file
Otwiera podany plik. Mo¿na równie¿ zamiast tego podaæ nazwê pliku jako niekwalifikowany argument, ale je¶li nazwa zaczyna siê od \-, nale¿y poprzedziæ j± \-\-, np. 'scribus \-\- \-myfile.sla'.
.TP 
.B \-h, \-\-help
Wy¶wietla krótk± instrukcjê obs³ugi.
.SH "¦RODOWISKO"
Scribus uwzglêdnia stanardowe zmienne ¶rodowiskowe zdefiniowane w locale. Inne zmienne ¶rodowiskowe mog± byæ u¿ywane przez programy i bibioteki wymagane przez Scribusa, np. QT.
.I http://docs.scribus.net/
oraz dokumentacja zawarta w programie mo¿e wymieniaæ inne zmienne, u¿ywane przez Scribusa i wymagane przez niego programy i biblioteki.
.TP 
.B LC_ALL, LC_MESSAGES, LANG
POSIX locale. Zobacz
.I locale(1).
Scribus stosuje te zmienne w kolejno¶ci podanej powy¿ej, aby wybraæ jêzyk (w³±cznie z t³umaczeniem interfejsu), który ma byæ u¿yty. Je¶li Scribus nie znajdzie ¿adnej z tych zmiennych, u¿yje ustawieñ locale stosowanych przez bibliotekê QT.
.TP 
.B PATH
Scribus mo¿e poszukaæ w PATH zewnêtrznych narzêdzi, je¶li ¶cie¿ki do nich nie zosta³y zdefiniowane w programie. Aktualnie ma to miejsce, kiedy zainstalowanych jest parê kopii gs(1). Mo¿na podaæ w³a¶ciw± ¶cie¿kê do gs(1) w ustawieniach Scribusa i obej¶æ w ten sposób przeszukiwanie 
.B PATH
.
.PP 
Poni¿ej znajduje siê zestawienie kilku najwa¿niejszych zmiennych ¶rodowiskowych u¿ywanych przez inne programy, jednak w przypadku problemów nale¿y zajrzeæ do oryginalnej dokumentacji programu.
.TP 
.B GS_FONTPATH
¦cie¿ka do czcionek u¿ywana przez GhostScript. Ma wp³yw na szukanie czcionek dla GhostScriptu, którego Scribus u¿ywa do wielu operacji na plikach PostScript. Dodawaj nowe katalogi zawieraj±ce czcionki rozdzielaj±c je ¶rednikiem, aby umo¿liwiæ GhostScriptowi znalezienie czcionek po³o¿onych w niestandardowych katalogach. Zobacz gs(1) i dokumentacjê GhostScriptu w formacie HTML, aby znale¼æ wiêcej informacji.
.TP 
.B GS_LIB
¦cie¿ka do biblioteki GhostScriptu. GhostScript szuka w tej ¶cie¿ce plików Fontmap. Podobnie jak
.B GS_FONTPATH
jest to lista katalogów rozdzielonych ¶rednikami. Zazwyczaj u¿ywa siê zamiast tego zmiennej 
.B GS_FONTPATH
, jednak utworzenie plików Fontmap i u¿ycie zmiennej 
.B GS_LIB
mo¿e przy¶pieszyæ dzia³anie, je¶li u¿ywamy 
.I bardzo du¿o
czcionek. Zobacz gs(1) i dokumentacjê GhostScriptu w formacie HTML, aby uzyskaæ wiêcej informacji.
.SH "PLIKI"
.TP 
.B $HOME/.scribus/
Ustawienia u¿ytkownika zapisywane s± w katalogu $HOME/.scribus/. Wiêkszo¶æ innych ¶cie¿ek mo¿na zmieniæ w ustawieniach Scribusa.
.TP 
.B $HOME/.scribus/scribus.rc
Ustawienia Scribusa we w³asnym formacie xml. Mo¿e zostaæ zast±piony nowym formatem pliku ustawieñ.
.TP 
.B $HOME/.scribus/prefs.xml
Plik ustawieñ w nowym formacie. W momencie tworzenia niniejszego dokumentu (wersja 1.2.x) plik ten jest jeszcze prawie ca³kowicie niewykorzystywany.
Pliki konfiguracyjne powi±zanych programów:
.TP 
.B $HOME/.fonts
.TP 
.B /etc/fonts
Fontconfig, biblioteka u¿ywana przez Scribusa do znajdowania czcionek, u¿ywa plików konfiguracyjnych znajduj±cych siê normalnie w katalogu /etc/fonts, g³ównie plików /etc/fonts/fonts.conf i /etc/fonts/local.conf. Mo¿e równie¿ u¿ywaæ pliku konfiguracyjnego w $HOME/.fonts/ . Zobacz fonts.conf(5) i dokumentacjê biblioteki fontconfig, aby uzyskaæ wiêcej informacji. Zwróæ uwagê, ¿e Scribus mo¿e równie¿ u¿ywaæ swoich w³asnych ¶cie¿ek do czcionek - zobacz  menu Ustawienia i pomoc wewn±trz Scribusa.
.SH "ZOBACZ TAK¯E"
Strona z dokumentacj± Scribusa http://docs.scribus.net/ i strona domowa http://www.scribus.net/

gs(1), dokumentacja gs w html i strona http://www.ghostscript.com/

.PP 
Biblioteka QT \- http://www.trolltech.com/

fonts\-conf(5) - informacje o konfiguracji biblioteki FontConfig
.SH "POKREWNE OPROGRAMOWANIE I PARTNERZY"
inkscape(1) \- http://inkscape.org/

OpenClipArt \- http://openclipart.org/

Fontmatrix \- http://fontmatrix.be/

Inni partnerzy mog± byæ wymienieni w stopce strony http://www.scribus.net/

gimp(1) \- http://www.gimp.org/

.SH "B£ÊDY"
Strona
.I 
http://bugs.scribus.net/
zawiera system ¶ledzenia b³êdów Scribusa, u¿ywany do zg³aszania b³êdów i propozycji zmian.
.B Je¶li masz zamiar zg³osiæ b³±d, przeszukaj najpierw bazê danych.
.SH "AUTORZY"
W menu \'Pomoc\' w opcji \'O Scribusie\' znale¼æ mo¿na listê autorów, t³umaczy i osób zaanga¿owanych w rozwój programu.
