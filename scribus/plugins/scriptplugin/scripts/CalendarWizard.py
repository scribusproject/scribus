# -*- coding: utf-8 -*-

""" This is a simple 'Calendar creation wizard' for Scribus. It's a fully
rewritten Calender.py from Scribus examples. Enjoy.

DESCRIPTION & USAGE:
This script needs Tkinter. It will create a GUI with available options
for easy calendar page creation. You'll get new pages with calendar
tables into a new document you are asked for. Position of the
objects in page is calculated with the "golden-ratio" aspect from the
page margins.

Steps to create:
    1) Fill requested values in the Calendar dialog
    2) You will be prompted for new document
    3) You will be prompted for new paragraph style which will be used
       in calendar text frames. It could be changed later.

There are 2 types of calender supported:
    1) Classic calendar with one month matrix per page. I suggest
       here PORTRAIT orientation.
    2) Event calendar with one week per page with huge place for
       human inputs. There should be LANDSCAPE imho.

But everything works with both orientations well of course too.

AUTHOR:
    Petr Vanek <petr@scribus.info>

LICENSE:
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

"""

import sys
import calendar
import datetime

try:
    from scribus import *
except ImportError:
    print "This Python script is written for the Scribus scripting interface."
    print "It can only be run from within Scribus."
    sys.exit(1)

try:
    # I wish PyQt installed everywhere :-/
    from Tkinter import *
    from tkFont import Font
except ImportError:
    print "This script requires Python's Tkinter properly installed."
    messageBox('Script failed',
               'This script requires Python\'s Tkinter properly installed.',
               ICON_CRITICAL)
    sys.exit(1)


localization = {
'Czech' :
    [['Leden', 'Únor', 'Březen', 'Duben', 'Květen',
        'Červen', 'Červenec', 'Srpen', 'Září',
        'Říjen', 'Listopad', 'Prosinec'],
     ['Pondělí','Úterý','Středa','Čtvrtek','Pátek','Sobota', 'Neděle']],
'Czech-short' :
    [['Leden', 'Únor', 'Březen', 'Duben', 'Květen',
        'Červen', 'Červenec', 'Srpen', 'Září',
        'Říjen', 'Listopad', 'Prosinec'],
     ['Po', 'Út', 'St', 'Čt', 'Pá', 'So', 'Ne']],
'Dutch' :
    [['Januari', 'Februari', 'Maart', 'April',
      'Mei', 'Juni', 'Juli', 'Augustus', 'September',
      'Oktober', 'November', 'December'],
     ['Maandag','Dinsdag','Woensdag','Donderdag','Vrijdag','Zaterdag', 'Zondag']],
# Dutch by "Christoph Schäfer" <christoph-schaefer@gmx.de>
'English' :
    [['January', 'February', 'March', 'April',
      'May', 'June', 'July', 'August', 'September',
      'October', 'November', 'December'],
     ['Monday','Tuesday','Wednesday','Thursday','Friday','Saturday', 'Sunday']],
'English-short' :
    [['January', 'February', 'March', 'April', 'May',
      'June', 'July', 'August', 'September', 'October',
      'November', 'December'],
     ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']],
'Finnish' :
    [['Tammikuu', 'Helmikuu', 'Maaliskuu', 'Huhtikuu',
      'Toukokuu', 'Kesäkuu', 'Heinäkuu', 'Elokuu', 'Syyskuu',
      'Lokakuu', 'Marraskuu', 'Joulukuu'],
     ['ma','ti','ke','to','pe','la', 'su']],
'French':
    [['Janvier', u'F\xe9vrier', 'Mars', 'Avril',
      'Mai', 'Juin', 'Juillet', u'Ao\xfbt', 'Septembre',
      'Octobre', 'Novembre', u'D\xe9cembre'],
     ['Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi','Dimanche']],
'German' :
    [['Januar', 'Februar', u'M\xe4rz', 'April',
      'Mai', 'Juni', 'Juli', 'August', 'September',
      'Oktober', 'November', 'Dezember'],
     ['Montag','Dienstag','Mittwoch','Donnerstag','Freitag','Samstag','Sonntag']],
'Italian' :
    [['Gennaio', 'Febbraio', 'Marzo', 'Aprile',
       'Maggio', 'Giugno', 'Luglio', 'Agosto', 'Settembre',
       'Ottobre', 'Novembre', 'Dicembre'],
    [u'Luned\xec', u'Marted\xec', u'Mercoled\xec', u'Gioved\exc', u'Venerd\xec', 'Sabato', 'Domenica']],
# Polish by "Łukasz [DeeJay1] Jernaś" <deejay1@nsj.srem.pl>
'Polish' :
    [['Styczeń', 'Luty', 'Marzec', 'Kwiecień', 'Maj',
      'Czerwiec', 'Lipiec', 'Sierpień', 'Wrzesień',
      'Październik', 'Listopad', 'Grudzień'],
     ['Poniedziałek', 'Wtorek', 'Środa', 'Czwartek', 'Piątek', 'Sobota', 'Niedziela']],
'Portuguese' :
    [['Janeiro', 'Fevereiro', u'Mar\xe7o', 'Abril',
      'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro',
      'Outubro', 'Novembro', 'Dezembro'],
     ['Segunda-feira', u'Ter\xe7a-feira', 'Quarta-feira', 'Quinta-feira', 'Sexta-feira', u'S\xe1bado', 'Domingo']],
'Russian' :
    [['Январь', 'Февраль', 'Март', 'Апрель',
      'Май', 'Июнь', 'Июль', 'Август', 'Сентябрь',
      'Октябрь', 'Ноябрь', 'Декабрь'],
     ['Понедельник','Вторник','Среда','Четверг','Пятница','Суббота', 'Воскресенье']],
'Slovak' :
    [['Január', 'Február', 'Marec', 'Apríl',
      'Máj', 'Jún', 'Júl', 'August', 'September',
      'Október', 'November', 'December'],
      ['Pondelok','Utorok','Streda','Štvrtok','Piatok','Sobota', 'Nedeľa']],
'Slovak-short' :
    [['Január', 'Február', 'Marec', 'Apríl',
      'Máj', 'Jún', 'Júl', 'August', 'September',
      'Október', 'November', 'December'],
      ['Po','Ut','St','Št','Pi','So', 'Ne']],
'Spanish' :
    [['Enero', 'Febrero', 'Marzo', 'Abril', 'Mayo',
      'Junio', 'Julio', 'Agosto', 'Septiembre',
      'Octubre', 'Noviembre', 'Diciembre'],
     ['Lunes', 'Martes', u'Mi\xe9rcoles', 'Jueves', 'Viernes', u'S\xe1bado', 'Domingo']],
'Swedish' :
    [['Januari', 'Februari','Mars', 'April','Maj', 'Juni','Juli', 'Augusti','September', 'Oktober', 'November', 'December'],
     ['Måndag', 'Tisdag','Onsdag', 'Torsdag','Fredag', 'Lördag','Söndag']]
}


from math import sqrt

class ScCalendar:
    """ Parent class for all calendar types """

    def __init__(self, year, months={}, dayOrder=[], firstDay=calendar.SUNDAY, drawSauce=True):
        """ Setup base things """
        # params
        self.drawSauce = drawSauce # draw supplementary image?
        self.year = year
        self.months = months
        self.dayOrder = dayOrder
        self.layerImg = 'Calendar image'
        self.layerCal = 'Calendar'
        self.pStyle = None # paragraph style
        # settings
        self.firstPage = True # create only 2nd 3rd ... pages. No 1st one.
        calendar.setfirstweekday(firstDay)
        progressTotal(len(months))

    def setupDocVariables(self):
        """ Compute base metrics here. Page layout is bordered by margins and
        virtually divided by golden mean 'cut' in the bottom. The calendar is
        in the bottom part - top is occupied with empty image frame. """
        page = getPageSize()
        self.pagex = page[0]
        self.pagey = page[1]
        marg = getPageMargins()
        self.marginl = marg[0]
        self.marginr = marg[1]
        self.margint = marg[2]
        self.marginb = marg[3]
        self.width = self.pagex - self.marginl - self.marginr
        self.height = self.pagey - self.margint - self.marginb

    def goldenMean(self, aSize):
        """ Taken from samples/golden-mean.py."""
        return aSize * ((sqrt(5) - 1)/2)

    def applyTextToFrame(self, aText, aFrame):
        """ Insert the text with style. """
        setText(aText, aFrame)
        setStyle(self.pStyle, aFrame)

    def createCalendar(self):
        """ Walk throudh months dict and calls monthly sheet """
        if not newDocDialog():
            return 'Create a new document first, please'
        self.pStyle = newStyleDialog()
        if  self.pStyle == None:
            closeDoc()
            return 'Create a default paragraph style, please'
        originalUnit = getUnit()
        setUnit(UNIT_POINTS)
        self.setupDocVariables()
        if self.drawSauce:
            createLayer(self.layerImg)
        createLayer(self.layerCal)
        run = 0
        for i in self.months:
            run += 1
            progressSet(run)
            self.createMonthCalendar(i)
        setUnit(originalUnit)
        return None

    def createLayout(self):
        """ Create the page and optional bells and whistless around """
        self.createPage()
        if self.drawSauce:
            setActiveLayer(self.layerImg)
            self.createImage()
        setActiveLayer(self.layerCal)

    def createPage(self):
        """ Wrapper to the new page with layers """
        if self.firstPage:
            self.firstPage = False
            gotoPage(1)
            return
        newPage(-1)


class ScEventCalendar(ScCalendar):
    """ One day = one row calendar. I suggest LANDSCAPE orientation.\
        One week per page."""

    def __init__(self, year, months = {}, dayOrder = [], firstDay = calendar.SUNDAY, drawSauce=True):
        ScCalendar.__init__(self, year, months, dayOrder, firstDay, drawSauce)

    def setupDocVariables(self):
        """ Compute base metrics here. Page layout is bordered by margins and
        virtually divided by golden mean 'cut' in the bottom. The calendar is
        in the bottom part - top is occupied with empty image frame. """
        # gloden mean
        ScCalendar.setupDocVariables(self)
        self.gmean = self.width - self.goldenMean(self.width) + self.marginl
        # calendar size = gmean
        # rows and cols
        self.rowSize = self.height / 8

    def createMonthCalendar(self, month):
        cal = calendar.monthcalendar(self.year, month + 1)
        for i in cal:
            self.createLayout()
            self.createHeader(self.months[month])
            rowCnt = 1
            for j in i: # days
                cel = createText(self.gmean + self.marginl,
                                 self.margint + rowCnt * self.rowSize,
                                 self.width - self.gmean, self.rowSize)
                rowCnt += 1
                if j != 0:
                    self.applyTextToFrame(str(j), cel)

    def createHeader(self, monthName):
        cel = createText(self.gmean + self.marginl, self.margint,
                            self.width - self.gmean, self.rowSize)
        self.applyTextToFrame(monthName, cel)

    def createImage(self):
        """ Wrapper for everytime-the-same image frame. """
        if self.drawSauce:
            createImage(self.marginl, self.margint, self.gmean, self.height)

class ScClassicCalendar(ScCalendar):
    """ Calendar matrix creator itself. I suggest PORTRAIT orientation.
        One month per page."""

    def __init__(self, year, months = {}, dayOrder = [], firstDay = calendar.SUNDAY, drawSauce=True):
        ScCalendar.__init__(self, year, months, dayOrder, firstDay, drawSauce)

    def setupDocVariables(self):
        """ Compute base metrics here. Page layout is bordered by margins and
        virtually divided by golden mean 'cut' in the bottom. The calendar is
        in the bottom part - top is occupied with empty image frame. """
        # gloden mean
        ScCalendar.setupDocVariables(self)
        self.gmean = self.height - self.goldenMean(self.height) + self.margint
        # calendar size
        self.calHeight = self.height - self.gmean + self.margint
        # rows and cols
        self.rowSize = self.gmean / 8
        self.colSize = self.width / 7

    def createMonthCalendar(self, month):
        """ Draw one month calendar per page """
        cal = calendar.monthcalendar(self.year, month + 1)
        self.createLayout()
        self.createHeader(self.months[month])
        # weeks
        rowCnt = 2
        for i in cal:
            colCnt = 0
            for j in i: # days
                cel = createText(self.marginl + colCnt * self.colSize,
                                 self.calHeight + rowCnt * self.rowSize,
                                 self.colSize, self.rowSize)
                colCnt += 1
                if j != 0:
                    self.applyTextToFrame(str(j), cel)
            rowCnt += 1

    def createHeader(self, monthName):
        """ Draw calendar header. Month name and days of the week """
        header = createText(self.marginl, self.calHeight, self.width, self.rowSize)
        self.applyTextToFrame(monthName, header)
        colCnt = 0
        for i in self.dayOrder:
            cel = createText(self.marginl + colCnt * self.colSize,
                             self.calHeight + self.rowSize,
                             self.colSize, self.rowSize)
            self.applyTextToFrame(i, cel)
            colCnt += 1

    def createImage(self):
        """ Wrapper for everytime-the-same image frame. """
        if self.drawSauce:
            createImage(self.marginl, self.margint,
                        self.width, self.calHeight - self.margint)


class TkCalendar(Frame):
    """ GUI interface for Scribus calendar wizard.
        It's ugly and very simple. I can say I hate Tkinter :-/"""

    def __init__(self, master=None):
        """ Setup the dialog """
        # refernce to the localization dictionary
        self.key = 'English'
        Frame.__init__(self, master)
        self.grid()
        self.master.resizable(0, 0)
        self.master.title('Scribus Calendar Wizard')
        #define widgets
        self.statusVar = StringVar()
        self.statusLabel = Label(self, textvariable=self.statusVar)
        self.statusVar.set('Select Options and Values')
        # langs
        # change the height = to match number of langs.
        self.langLabel = Label(self, text='Select language:')

        self.langFrame = Frame(self)
        self.langFrame.pack()
        self.langScrollbar = Scrollbar(self.langFrame)
        self.langScrollbar.pack(fill=Y, side=RIGHT)
        self.langListbox = Listbox(self.langFrame, selectmode=SINGLE, height=10, yscrollcommand=self.langScrollbar.set)
        self.langListbox.pack(fill=X,side=LEFT)
        self.langScrollbar.config(command=self.langListbox.yview)

        keys = localization.keys()
        keys.sort()
        for i in keys:
            self.langListbox.insert(END, i)
        self.langButton = Button(self, text='Change language', command=self.languageChange)
        # calendar type
        self.typeLabel = Label(self, text='Calendar type')
        self.typeVar = IntVar()
        self.typeClRadio = Radiobutton(self, text='Classic', variable=self.typeVar, value=0)
        self.typeEvRadio = Radiobutton(self, text='Event', variable=self.typeVar, value=1)
        # start of week
        self.weekStartsLabel = Label(self, text='Week begins with:')
        self.weekVar = IntVar()
        self.weekMondayRadio = Radiobutton(self, text='Mon', variable=self.weekVar, value=calendar.MONDAY)
        self.weekSundayRadio = Radiobutton(self, text='Sun', variable=self.weekVar, value=calendar.SUNDAY)
        # year
        self.yearLabel = Label(self, text='Year:')
        self.yearVar = StringVar()
        self.yearEntry = Entry(self, textvariable=self.yearVar, width=4)
        self.wholeYearLabel = Label(self, text='Whole year:')
        self.wholeYear = IntVar()
        self.wholeYearCheck = Checkbutton(self, command=self.setWholeYear, variable=self.wholeYear)
        # months
        self.monthLabel = Label(self, text='Months:')
        self.monthListbox = Listbox(self, selectmode=MULTIPLE, height=12)
        # layout stuff
        self.imageLabel = Label(self, text='Draw Image Frame:')
        self.imageVar = IntVar()
        self.imageCheck = Checkbutton(self, variable=self.imageVar)
        # closing/running
        self.okButton = Button(self, text="OK", width=6, command=self.okButonn_pressed)
        self.cancelButton = Button(self, text="Cancel", command=self.quit)
        # setup values
        self.weekMondayRadio.select()
        self.typeClRadio.select()
        self.yearVar.set(str(datetime.date(1, 1, 1).today().year))
        self.imageCheck.select()
        # make layout
        self.columnconfigure(0, pad=6)
        currRow = 0
        self.statusLabel.grid(column=0, row=currRow, columnspan=4)
        currRow += 1
        self.langLabel.grid(column=0, row=currRow, sticky=W)
        self.monthLabel.grid(column=3, row=currRow, sticky=W)
        currRow += 1
        self.langFrame.grid(column=0, row=currRow, rowspan=6, sticky=N)
        self.typeLabel.grid(column=1, row=currRow, sticky=E)
        self.typeClRadio.grid(column=2, row=currRow, sticky=W)
        self.monthListbox.grid(column=3, row=currRow, rowspan=8)
        currRow += 1
        self.typeEvRadio.grid(column=2, row=currRow, sticky=W)
        currRow += 1
        self.weekStartsLabel.grid(column=1, row=currRow, sticky=N+E)
        self.weekMondayRadio.grid(column=2, row=currRow, sticky=N+W)
        currRow += 1
        self.weekSundayRadio.grid(column=2, row=currRow, sticky=N+W)
        currRow += 1
        self.yearLabel.grid(column=1, row=currRow, sticky=N+E)
        self.yearEntry.grid(column=2, row=currRow, sticky=N+W)
        currRow += 1
        self.wholeYearLabel.grid(column=1, row=currRow, sticky=N+E)
        self.wholeYearCheck.grid(column=2, row=currRow, sticky=N+W)
        currRow += 1
        self.imageLabel.grid(column=1, row=currRow, sticky=N+E)
        self.imageCheck.grid(column=2, row=currRow, sticky=N+W)
        self.langButton.grid(column=0, row=currRow)
        currRow += 3
        self.rowconfigure(currRow, pad=6)
        self.okButton.grid(column=1, row=currRow, sticky=E)
        self.cancelButton.grid(column=2, row=currRow, sticky=W)
        # fill the values
        self.realLangChange()

    def languageChange(self, lang='English'):
        """ Called by Change button. Get language list value and
            call real re-filling. """
        ix = self.langListbox.curselection()
        if len(ix)==0:
            self.statusVar.set('Select a language, please')
            return
        self.realLangChange(lang=self.langListbox.get(ix[0]))

    def realLangChange(self, lang='English'):
        """ Real widget setup. Ot takes values from localization dictionary.
        [0] = months, [1] Days """
        self.key = lang
        self.monthListbox.delete(0, END)
        self.wholeYear.set(0)
        for i in localization[lang][0]:
            self.monthListbox.insert(END, i)

    def setWholeYear(self):
        """ All/none months selection. It's called after "Whole year" check button
        click. """
        if self.wholeYear.get() == 1:
            self.monthListbox.selection_set(0, END)
        else:
            self.monthListbox.selection_clear(0, END)

    def okButonn_pressed(self):
        """ User variables testing and preparing """
        # year
        try:
            year = self.yearVar.get().strip()
            if len(year) != 4:
                raise ValueError
            year = int(year, 10)
        except ValueError:
            self.statusVar.set('Year must be in the "YYYY" format e.g. 2005.')
            return
        # months
        selMonths = self.monthListbox.curselection()
        if len(selMonths) == 0:
            self.statusVar.set('At least one month must be selected.')
            return
        months = {}
        for i in selMonths:
            months[int(i)] = self.monthListbox.get(i)
        # day order
        dayList = localization[self.key][1]
        if self.weekVar.get() == calendar.SUNDAY:
            dl = dayList[:6]
            dl.insert(0, dayList[6])
            dayList = dl
        # draw images etc.
        if self.imageVar.get() == 0:
            draw = False
        else:
            draw = True
        # create calendar (finally)
        if self.typeVar.get() == 0:
            cal = ScClassicCalendar(year, months, dayList, self.weekVar.get(), draw)
        else:
            cal = ScEventCalendar(year, months, dayList, self.weekVar.get(), draw)
        self.master.withdraw()
        err = cal.createCalendar()
        if err != None:
            self.master.deiconify()
            self.statusVar.set(err)
        else:
            self.quit()

    def quit(self):
        self.master.destroy()


def main():
    """ Application/Dialog loop with Scribus sauce around """
    try:
        statusMessage('Running script...')
        progressReset()
        root = Tk()
        app = TkCalendar(root)
        root.mainloop()
    finally:
        if haveDoc():
            redrawAll()
        statusMessage('Done.')
        progressReset()

if __name__ == '__main__':
    main()
