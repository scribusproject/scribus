from PyQt4.QtCore import pyqtSignature, Qt
from PyQt4.QtGui import QMainWindow, QSplitter, QTabWidget, QApplication, QFileDialog, QMessageBox, QCloseEvent

from widget import PythonEditorWidget,  QtScriptEditorWidget, SaveDialog
from console import PythonConsole, QtScriptConsole
from mainwindow_ui import Ui_ScriptEditor


import traceback
import os

template_py = """\
# -*- coding: utf-8 -*-
from __future__ import with_statement

"""

class EditorMainWindow(QMainWindow):


    def __init__(self, parent=None):
        QMainWindow.__init__(self, parent)
        self.ui = Ui_ScriptEditor()
        self.ui.setupUi(self)
        #self.connect(self.ui.actionExit, SIGNAL("triggered()"), self.exit)
        self.splitter = QSplitter(Qt.Vertical, self)
        self.setCentralWidget(self.splitter)
        self.edit_tab = QTabWidget(self.splitter)
        self.console_tab = QTabWidget(self.splitter)
        self.py_console = PythonConsole(self.console_tab)
        self.console_tab.addTab(self.py_console, "&Python console")
        self.js_console = QtScriptConsole(self.console_tab)
        self.console_tab.addTab(self.js_console, "&QtScript console")
        self.editors = []
        self.on_actionNewPython_triggered()

    @pyqtSignature("")
    def closeEvent(self, event):
	while(self.editors.__len__()):
	    edit = self.edit_tab.currentWidget()
            if edit:
	        if(edit.isModified()):
		    saveBox = SaveDialog("You have unsaved script. Save it now?")
		    prompt = saveBox.exec_()
		    if(prompt == QMessageBox.Save):
			event.ignore()
		        self.save(True)
		    elif(prompt == QMessageBox.Cancel):
		        event.ignore()
			return
		    elif(prompt == QMessageBox.Discard):
		        event.accept()
                i = self.edit_tab.indexOf(edit)
                self.edit_tab.removeTab(i)
                self.editors.remove(edit)
	event.accept()

	

    @pyqtSignature("")
    def on_actionExit_triggered(self):
	while(self.editors.__len__()):
	    edit = self.edit_tab.currentWidget()
            if edit:
	        if(edit.isModified()):
		    saveBox = SaveDialog("You have unsaved script. Save it now?")
		    prompt = saveBox.exec_()
		    if(prompt == QMessageBox.Save):
		        self.save(True)
		    elif(prompt == QMessageBox.Cancel):
		        return
		    elif(prompt == QMessageBox.Discard):
		        pass
		i = self.edit_tab.indexOf(edit)
                self.edit_tab.removeTab(i)
                self.editors.remove(edit)
	self.close()

    @pyqtSignature("")
    def on_actionNewPython_triggered(self):
        pyedit = PythonEditorWidget(self.edit_tab)
        pyedit.setPlainText(template_py)
        self.edit_tab.addTab(pyedit, "Python")
        self.edit_tab.setCurrentWidget(pyedit)
        self.editors.append(pyedit)
        self.py_console.attach()
        self.console_tab.setCurrentIndex(0)
        pyedit.setFocus()
        pyedit.view.setFocus()


    @pyqtSignature("")
    def on_actionNewQtScript_triggered(self):
        jsedit = QtScriptEditorWidget(self.edit_tab)
        self.edit_tab.addTab(jsedit, "QtScript")
        self.edit_tab.setCurrentWidget(jsedit)
        self.editors.append(jsedit)
        self.js_console.attach()
        self.console_tab.setCurrentIndex(1)


    @pyqtSignature("")
    def on_actionClose_triggered(self):
        edit = self.edit_tab.currentWidget()
        if edit:
	    if(edit.isModified()):
		saveBox = SaveDialog("Do you want to save this Script?")
		prompt = saveBox.exec_()
		if(prompt == QMessageBox.Save):
		    self.save(True)
		elif(prompt == QMessageBox.Cancel):
		    return
		elif(prompt == QMessageBox.Discard):
		    pass
            i = self.edit_tab.indexOf(edit)
            self.edit_tab.removeTab(i)
            self.editors.remove(edit)

        
    @pyqtSignature("")
    def on_actionClear_triggered(self):
        #edit = self.edit_tab.currentWidget()
	#edit.setPlainText(template_py)
	self.py_console.clear()


    @pyqtSignature("")
    def on_actionSave_As_triggered(self):
	self.save()


    @pyqtSignature("")
    def on_actionSave_triggered(self):
	self.save(True)


    #Path of the script file in each tab will be stored in tabToolTip
    def save(self, Update = False):
        edit = self.edit_tab.currentWidget()
	contents = str(edit.toPlainText())
	if((Update == False) or (self.edit_tab.tabText(self.edit_tab.currentIndex()) == "Python") ):
	    #Save in its first invocation and Save As will enter  
	    filename = QFileDialog.getSaveFileName(self, "Save File", "", "*.spy")
	    fil = open(filename , 'w')
	    if(filename and self.edit_tab.tabText(self.edit_tab.currentIndex()) == "Python"):
		#Script hasn't been saved before and user specifies a valid filename
	        self.edit_tab.setTabToolTip(self.edit_tab.currentIndex(), filename+'.spy')
	        self.edit_tab.setTabText(self.edit_tab.currentIndex(), os.path.basename(str(filename+'.spy')))
	else:
	    #filename = self.edit_tab.tabText(self.edit_tab.currentIndex())
	    filename = self.edit_tab.tabToolTip(self.edit_tab.currentIndex())
	    fil = open( filename , 'w')
	fil.write(contents)	
	fil.close()
	edit.setModified(False)


    @pyqtSignature("")
    def on_actionOpen_triggered(self):
	filename = QFileDialog.getOpenFileName(self,"Open File","","*.spy")
	try:
	    fil = open(filename , 'r')
	except IOError:
	    return
	code = fil.read()
	edit = self.edit_tab.currentWidget()
	self.edit_tab.setTabText(self.edit_tab.currentIndex(), os.path.basename(str(filename)))
	self.edit_tab.setTabToolTip(self.edit_tab.currentIndex(), filename)
	edit.setPlainText(code)
	fil.close()


    @pyqtSignature("")
    def on_actionRun_triggered(self):
        self.run()


    @pyqtSignature("")
    def on_actionRunConsole_triggered(self):
        self.run(True)


    def run(self, console=False):
        edit = self.edit_tab.currentWidget()
        code = str(edit.toPlainText())
        if isinstance(edit, PythonEditorWidget):
            self.py_console.attach()
            self.console_tab.setCurrentIndex(0)
            if console:
                namespace = self.py_console.namespace
            else:
                namespace = {}
            try:
                exec code in namespace
            except Exception, e:
                traceback.print_exc()
            try:
                Scripter.activeWindow.redraw = True
                Scripter.activeWindow.update()
            except: pass
        else:
            self.js_console.attach()
            self.console_tab.setCurrentIndex(1)
            if console:
                self.js_console.inter.execute(code)
            else:
                self.js_console.inter.execute_code(code)


        
        
if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    win = EditorMainWindow()
    win.resize(640, 480)
    win.show()
    app.exec_()
