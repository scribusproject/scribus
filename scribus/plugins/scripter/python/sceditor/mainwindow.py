from PyQt4.QtCore import pyqtSignature, Qt
from PyQt4.QtGui import QMainWindow, QSplitter, QTabWidget, QApplication

from sceditor.widget import PythonEditorWidget,  QtScriptEditorWidget
from sceditor.console import PythonConsole, QtScriptConsole
from sceditor.mainwindow_ui import Ui_ScriptEditor


import traceback


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
    def on_actionExit_triggered(self):
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
            i = self.edit_tab.indexOf(edit)
            self.edit_tab.removeTab(i)
            self.editors.remove(edit)
        


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
