""" This Script creates a Calendar Sheet for the Current Month """

import calendar
import time
from scribus import *
if HaveDoc():
	SetRedraw(0)
	Month = time.localtime()[1]
	Year = time.localtime()[0]
	Objects = []
	MonthList = ["January","February","March","April","May","June","July","August","September","October","November","December"]
	DaysList = ["Mon","Tue","Wed","Thu","Fri","Sat","Sun"]
	Xcoor = 10
	Ycoor = 30
	DayC = 0
	Calend = calendar.monthcalendar(Year, Month)
	ob = CreateText(10, 10, 245, 20)
	Title = MonthList[Month-1] + " " + str(Year)
	SetText(Title, ob)
	Objects.append(ob)
	for lx in range(45, 245, 35):
		ob = CreateLine(lx, 30, lx, 20*len(Calend)+50)
		Objects.append(ob)
	for ly in range(50, 20*len(Calend)+50, 20):
		ob = CreateLine(10, ly, 255, ly)
		Objects.append(ob)
	ob = CreateRect(10, 30, 245, 20*len(Calend)+20)
	SetFillColor("None", ob)
	Objects.append(ob)
	for day in range(7):
		ob = CreateText(Xcoor, Ycoor, 35, 20)
		SetTextAlignment(Centered, ob)
		SetFontSize(12, ob)
		if day == 6:
			SetTextColor("Red", ob)
		SetText(DaysList[day], ob)
		Objects.append(ob)
		Xcoor = Xcoor + 35
	Ycoor = Ycoor + 20
	for lines in Calend:
		Xcoor = 10
		DayC = 0
		for rows in lines:
			if rows != 0:
				ob = CreateText(Xcoor, Ycoor, 35, 20)
				SetTextAlignment(Centered, ob)
				if DayC == 6:
					SetTextColor("Red", ob)
				SetText(str(rows), ob)
				Objects.append(ob)
			Xcoor = Xcoor + 35
			DayC = DayC + 1
		Ycoor = Ycoor + 20
	GroupObjects(Objects)
	SetRedraw(1)
	RedrawAll
