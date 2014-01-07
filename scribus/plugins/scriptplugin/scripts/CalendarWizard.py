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

There are 2 types of calendar supported:
    1) Classic calendar with one month matrix per page. I suggest
       here PORTRAIT orientation.
    2) Horizontal event calendar with one week per page with huge place
       for human inputs. There should be LANDSCAPE imho.
    3) Horizontal event calendar with one week per page with huge place
       for human inputs. There should be LANDSCAPE imho.

But everything works with both orientations well of course too.

AUTHORS:
    Petr Vanek <petr@scribus.info>
    Bernhard Reiter <ockham@raz.or.at>

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

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
'Catalan' :
    [['Gener', 'Febrer', 'Març', 'Abril', 'Maig',
      'Juny', 'Juliol', 'Agost', 'Setembre',
      'Octubre', 'Novembre', 'Desembre'],
     ['Dilluns', 'Dimarts', 'Dimecres', 'Dijous', 'Divendres', 'Dissabte', 'Diumenge']],
'Catalan-short' :
    [['Gener', 'Febrer', 'Març', 'Abril', 'Maig',
      'Juny', 'Juliol', 'Agost', 'Setembre',
      'Octubre', 'Novembre', 'Desembre'],
     ['Dl', 'Dm', 'Dc', 'Dj', 'Dv', 'Ds', 'Dg']],
# Catalan by "Cesc Morata" <atarom@gmail.com>
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
# Croatian by daweed
'Croatian' :
    [['Siječanj', 'Veljača', 'Ožujak', 'Travanj', 'Svibanj',
        'Lipanj', 'Srpanj', 'Kolovoz', 'Rujan',
        'Listopad', 'Studeni', 'Prosinac'],
     ['Ponedjeljak','Utorak','Srijeda','Četvrtak','Petak','Subota', 'Nedjelja']],

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
'German (Austrian)' :
    [[u'J\xe4nner', 'Feber', u'M\xe4rz', 'April',
      'Mai', 'Juni', 'Juli', 'August', 'September',
      'Oktober', 'November', 'Dezember'],
     ['Montag','Dienstag','Mittwoch','Donnerstag','Freitag','Samstag','Sonntag']],
# Hungarian by Gergely Szalay szalayg@gmail.com	      
'Hungarian' :
    [['Január', 'Február', 'Március', 'Április',
       'Május', 'Június', 'Július', 'Augusztus', 'Szeptember',
       'Október', 'November', 'December'],
    ['Hétfő','Kedd','Szerda','Csütörtök','Péntek','Szombat','Vasárnap']],
'Italian' :
    [['Gennaio', 'Febbraio', 'Marzo', 'Aprile',
       'Maggio', 'Giugno', 'Luglio', 'Agosto', 'Settembre',
       'Ottobre', 'Novembre', 'Dicembre'],
    [u'Luned\xec', u'Marted\xec', u'Mercoled\xec', u'Gioved\xec', u'Venerd\xec', 'Sabato', 'Domenica']],
# Norwegian by Joacim Thomassen joacim@net.homelinux.org
'Norwegian' :
    [['Januar', 'Februar','Mars', 'April','Mai', 'Juni','Juli', 'August','September', 'Oktober', 'November', 'Desember'],
     ['Mandag', 'Tirsdag','Onsdag', 'Torsdag','Fredag', 'Lørdag','Søndag']],
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
# Romanian by Costin Stroie <costinstroie@eridu.eu.org>
'Romanian' :
    [['Ianuarie', 'Februarie', 'Martie', 'Aprilie',
      'Mai', 'Iunie', 'Iulie', 'August', 'Septembrie',
      'Octombrie', 'Noiembrie', 'Decembrie'],
     ['Luni','Mar\xc8\x9bi','Miercuri','Joi','Vineri','S\xc3\xa2mb\xc4\x83t\xc4\x83', 'Duminic\xc4\x83']],
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

    def __init__(self, year, months=[], firstDay=calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        """ Setup basic things """
        # params
        self.drawSauce = drawSauce # draw supplementary image?
        self.year = year
        self.months = months
        self.lang = lang
        # day order
        self.dayOrder = localization[self.lang][1]
        if firstDay == calendar.SUNDAY:
            dl = self.dayOrder[:6]
            dl.insert(0, self.dayOrder[6])
            self.dayOrder = dl
        self.mycal = calendar.Calendar(firstDay)
        self.layerImg = 'Calendar image'
        self.layerCal = 'Calendar'
        self.pStyleDate = "Date" # paragraph styles
        self.pStyleWeekday = "Weekday"
        self.pStyleMonth = "Month"
        self.pStyleWeekNo = "WeekNo"
        self.masterPage = "Weekdays"
        self.sepMonths = sepMonths
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
        # See http://docs.scribus.net/index.php?lang=en&page=scripterapi-page#-getPageMargins
        self.margint = marg[0]
        self.marginl = marg[1]
        self.marginr = marg[2]
        self.marginb = marg[3]
        self.width = self.pagex - self.marginl - self.marginr
        self.height = self.pagey - self.margint - self.marginb

    def goldenMean(self, aSize):
        """ Taken from samples/golden-mean.py."""
        return aSize * ((sqrt(5) - 1)/2)

    def applyTextToFrame(self, aText, aFrame):
        """ Insert the text with style. """
        setText(aText, aFrame)
        setStyle(self.pStyleDate, aFrame)

    def createCalendar(self):
        """ Walk through months dict and call monthly sheet """
        if not newDocDialog():
            return 'Create a new document first, please'
        createParagraphStyle(name=self.pStyleDate, alignment=ALIGN_RIGHT)
        createParagraphStyle(name=self.pStyleWeekday, alignment=ALIGN_RIGHT)
        createParagraphStyle(name=self.pStyleMonth)
        createParagraphStyle(name=self.pStyleWeekNo, alignment=ALIGN_RIGHT)
        originalUnit = getUnit()
        setUnit(UNIT_POINTS)
        self.setupDocVariables()
        if self.drawSauce:
            createLayer(self.layerImg)
        createLayer(self.layerCal)
        self.setupMasterPage()
        run = 0
        for i in self.months:
            run += 1
            progressSet(run)
            cal = self.mycal.monthdatescalendar(self.year, i + 1)
            self.createMonthCalendar(i, cal)
        setUnit(originalUnit)
        return None

    def createLayout(self):
        """ Create the page and optional bells and whistles around """
        self.createPage()
        if self.drawSauce:
            setActiveLayer(self.layerImg)
            self.createImage()
        setActiveLayer(self.layerCal)

    def createPage(self):
        """ Wrapper to the new page with layers """
        if self.firstPage:
            self.firstPage = False
            newPage(-1, self.masterPage) # create a new page using the masterPage
            deletePage(1) # now it's safe to delete the first page
            gotoPage(1)
            return
        newPage(-1, self.masterPage)

class ScEventCalendar(ScCalendar):
    """ Parent class for event
        (horizontal event, vertical event) calendar types """

    def __init__(self, year, months = [], firstDay = calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        ScCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)

    def printMonth(self, cal, month, week):
	    """ Print the month name(s) """
	    if week[6].day < 7:
		    if (week == cal[len(cal)-1]):
			    self.createHeader(localization[self.lang][0][month] + self.sepMonths + localization[self.lang][0][(month+1)%12])
		    elif ((month-1) not in self.months):
			    self.createHeader(localization[self.lang][0][(month-1)%12] + self.sepMonths + localization[self.lang][0][month])
	    else:
		    self.createHeader(localization[self.lang][0][month])

    def createMonthCalendar(self, month, cal):
        """ Draw one week calendar per page """
        for week in cal:
            # Avoid duplicate week around the turn of the months:
            # Only include week:
            # * If it's not the first week in a month, or, if it is:
            # * If it starts on the first weekday
            # * If the month before it isn't included
            if (week != cal[0]) or (week[0].day == 1) or ((month-1) not in self.months):
				self.createLayout()
				self.printMonth(cal, month, week)
				self.printWeekNo(week)

				for day in week:
				    self.printDay(day)

class ScHorizontalEventCalendar(ScEventCalendar):
    """ One day = one row calendar. I suggest LANDSCAPE orientation.\
        One week per page."""

    def __init__(self, year, months = [], firstDay = calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        ScEventCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)

    def setupDocVariables(self):
        """ Compute base metrics here. Page layout is bordered by margins and
        virtually divided by golden mean 'cut' in the bottom. The calendar is
        in the bottom part - top is occupied with empty image frame. """
        # golden mean
        ScCalendar.setupDocVariables(self)
        self.gmean = self.width - self.goldenMean(self.width) + self.marginl
        # calendar size = gmean
        # rows and cols
        self.rowSize = self.height / 8

    def printWeekNo(self, week):
        """ Dummy for now
            (for this type of calendar - see ScVerticalEventCalendar) """
        return

    def printDay(self, j):
        """ Print a given day """
        cel = createText(self.gmean + self.marginl,
                         self.margint + (1 + (j.weekday()-calendar.firstweekday())%7) * self.rowSize,
                         self.width - self.gmean, self.rowSize)
        setText(str(j.day), cel)
        setStyle(self.pStyleDate, cel)

    def createHeader(self, monthName):
        """ Draw calendar header: Month name """
        cel = createText(self.gmean + self.marginl, self.margint,
                            self.width - self.gmean, self.rowSize)
        setText(monthName, cel)
        setStyle(self.pStyleMonth, cel)

    def createImage(self):
        """ Wrapper for everytime-the-same image frame. """
        if self.drawSauce:
            createImage(self.marginl, self.margint, self.gmean, self.height)

    def setupMasterPage(self):
        """ Create a master page (not used for this type of calendar """
        createMasterPage(self.masterPage)
        closeMasterPage()

class ScVerticalCalendar(ScCalendar):
    """ Parent class for vertical
        (classic, vertical event) calendar types """

    def __init__(self, year, months = [], firstDay = calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        ScCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)

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

    def setupMasterPage(self):
        """ Draw invariant calendar header: Days of the week """
        createMasterPage(self.masterPage)
        editMasterPage(self.masterPage)
        setActiveLayer(self.layerCal)
        rowCnt = 0
        for j in self.dayOrder: # days
            cel = createText(self.marginl + rowCnt*self.colSize,
                             self.calHeight + self.rowSize,
                             self.colSize, self.rowSize)
            setText(j, cel)
            setStyle(self.pStyleWeekday, cel)
            rowCnt+=1
        closeMasterPage()

    def createHeader(self, monthName):
        """ Draw calendar header: Month name """
        header = createText(self.marginl, self.calHeight, self.width, self.rowSize)
        setText(monthName, header)
        setStyle(self.pStyleMonth, header)

    def createImage(self):
        """ Wrapper for everytime-the-same image frame. """
        if self.drawSauce:
            createImage(self.marginl, self.margint,
                        self.width, self.calHeight - self.margint)

class ScClassicCalendar(ScVerticalCalendar):
    """ Calendar matrix creator itself. I suggest PORTRAIT orientation.
        One month per page."""

    def __init__(self, year, months = [], firstDay = calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        ScVerticalCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)

    def createMonthCalendar(self, month, cal):
        """ Create a page and draw one month calendar on it """
        self.createLayout()
        self.createHeader(localization[self.lang][0][month])
        rowCnt = 2
        for week in cal:
            colCnt = 0
            for day in week:
                cel = createText(self.marginl + colCnt * self.colSize,
                                 self.calHeight + rowCnt * self.rowSize,
                                 self.colSize, self.rowSize)
                colCnt += 1
                if day.month == month + 1:
					setText(str(day.day), cel)
					setStyle(self.pStyleDate, cel)
            rowCnt += 1

class ScVerticalEventCalendar(ScVerticalCalendar, ScEventCalendar):
    """ One day = one column calendar. I suggest LANDSCAPE orientation.\
        One week per page."""

    def __init__(self, year, months = [], firstDay = calendar.SUNDAY, drawSauce=True, sepMonths='/', lang='English'):
        ScVerticalCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)
        ScEventCalendar.__init__(self, year, months, firstDay, drawSauce, sepMonths, lang)

    def printDay(self, j):
        """ Print a given day """
        cel = createText(self.marginl + ((j.weekday()-calendar.firstweekday())%7)*self.colSize,
                         self.calHeight + self.rowSize,
                         self.colSize/5, self.rowSize)
        setText(str(j.day), cel)
        setStyle(self.pStyleDate, cel)

    def printWeekNo(self, week):
        """ Print the week number for the given week"""
        weekCel = createText(self.marginl, self.calHeight, self.width, self.rowSize)
        # Week number: of this week's Thursday.
        # See http://docs.python.org/library/datetime.html#datetime.date.isocalendar
        # Note that week calculation isn't perfectly universal yet:
        # http://en.wikipedia.org/wiki/Week_number#Week_number
        setText(str(week[(calendar.THURSDAY-calendar.firstweekday())%7].isocalendar()[1]), weekCel)
        setStyle(self.pStyleWeekNo, weekCel)

class TkCalendar(Frame):
    """ GUI interface for Scribus calendar wizard.
        It's ugly and very simple. I can say I hate Tkinter :-/"""

    def __init__(self, master=None):
        """ Setup the dialog """
        # reference to the localization dictionary
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
        self.langFrame.grid()
        self.langScrollbar = Scrollbar(self.langFrame, orient=VERTICAL)
        self.langScrollbar.grid(row=0, column=1, sticky=N+S)
        self.langListbox = Listbox(self.langFrame, selectmode=SINGLE, height=10, yscrollcommand=self.langScrollbar.set)
        self.langListbox.grid(row=0, column=0, sticky=N+S+E+W)
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
        self.typeEvRadio = Radiobutton(self, text='Event (Horizontal)', variable=self.typeVar, value=1)
        self.typeVERadio = Radiobutton(self, text='Event (Vertical)', variable=self.typeVar, value=2)
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
        # Months separator
        self.sepMonthsLabel = Label(self, text='Months separator:')
        self.sepMonthsVar = StringVar()
        self.sepMonthsEntry = Entry(self, textvariable=self.sepMonthsVar, width=4)
        # closing/running
        self.okButton = Button(self, text="OK", width=6, command=self.okButonn_pressed)
        self.cancelButton = Button(self, text="Cancel", command=self.quit)
        # setup values
        self.weekMondayRadio.select()
        self.typeClRadio.select()
        self.yearVar.set(str(datetime.date(1, 1, 1).today().year))
        self.sepMonthsVar.set('/')
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
        self.typeVERadio.grid(column=2, row=currRow, sticky=W)
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
        currRow += 1
        self.sepMonthsLabel.grid(column=1, row=currRow, sticky=N+E)
        self.sepMonthsEntry.grid(column=2, row=currRow, sticky=N+W)
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
        months = []
        for i in selMonths:
            months.append(int(i))
        # draw images etc.
        if self.imageVar.get() == 0:
            draw = False
        else:
            draw = True
        # create calendar (finally)
        if self.typeVar.get() == 0:
            cal = ScClassicCalendar(year, months, self.weekVar.get(), draw, self.sepMonthsVar.get(), self.key)
        elif self.typeVar.get() == 1:
            cal = ScHorizontalEventCalendar(year, months, self.weekVar.get(), draw, self.sepMonthsVar.get(), self.key)
        else:
            cal = ScVerticalEventCalendar(year, months, self.weekVar.get(), draw, self.sepMonthsVar.get(), self.key)
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
        if haveDoc() > 0:
            redrawAll()
        statusMessage('Done.')
        progressReset()

if __name__ == '__main__':
    main()

