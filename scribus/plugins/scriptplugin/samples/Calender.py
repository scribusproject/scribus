#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" This Script creates a Calendar Sheet for the Current Month """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

import calendar
import time

def main():
    Month = time.localtime()[1]
    Year = time.localtime()[0]
    Objects = []
    MonthList = ["January","February","March","April","May","June","July","August","September","October","November","December"]
    DaysList = ["Mon","Tue","Wed","Thu","Fri","Sat","Sun"]
    Xcoor = 10
    Ycoor = 30
    DayC = 0
    Calend = calendar.monthcalendar(Year, Month)
    ob = createText(10, 10, 245, 20)
    Title = MonthList[Month-1] + " " + str(Year)
    setText(Title, ob)
    Objects.append(ob)
    for lx in range(45, 245, 35):
        ob = createLine(lx, 30, lx, 20*len(Calend)+50)
        Objects.append(ob)
    for ly in range(50, 20*len(Calend)+50, 20):
        ob = createLine(10, ly, 255, ly)
        Objects.append(ob)
    ob = createRect(10, 30, 245, 20*len(Calend)+20)
    setFillColor("None", ob)
    Objects.append(ob)
    for day in range(7):
        ob = createText(Xcoor, Ycoor, 35, 20)
        setTextAlignment(ALIGN_CENTERED, ob)
        setFontSize(12, ob)
        if day == 6:
            setTextColor("Red", ob)
        setText(DaysList[day], ob)
        Objects.append(ob)
        Xcoor = Xcoor + 35
    Ycoor = Ycoor + 20
    for lines in Calend:
        Xcoor = 10
        DayC = 0
        for rows in lines:
            if rows != 0:
                ob = createText(Xcoor, Ycoor, 35, 20)
                setTextAlignment(ALIGN_CENTERED, ob)
                if DayC == 6:
                    setTextColor("Red", ob)
                setText(str(rows), ob)
                Objects.append(ob)
            Xcoor = Xcoor + 35
            DayC = DayC + 1
        Ycoor = Ycoor + 20
    groupObjects(Objects)

if __name__ == '__main__':
    if haveDoc():
        try:
            setRedraw(False)
            main()
        finally:
            setRedraw(True)
            redrawAll()
    else:
        messageBox("Calendar Script", "Please run this script with a document open.", ICON_INFORMATION);
