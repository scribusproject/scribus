# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created: Sat Jun 14 05:22:25 2008
#      by: PyQt4 UI code generator 4.4.2
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ScriptEditor(object):
    def setupUi(self, ScriptEditor):
        ScriptEditor.setObjectName("ScriptEditor")
        ScriptEditor.resize(624,449)
        self.centralwidget = QtGui.QWidget(ScriptEditor)
        self.centralwidget.setGeometry(QtCore.QRect(0,28,624,397))
        self.centralwidget.setObjectName("centralwidget")
        ScriptEditor.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(ScriptEditor)
        self.menubar.setGeometry(QtCore.QRect(0,0,624,28))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menu_New = QtGui.QMenu(self.menuFile)
        self.menu_New.setObjectName("menu_New")
        self.menuRun = QtGui.QMenu(self.menubar)
        self.menuRun.setObjectName("menuRun")
        ScriptEditor.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(ScriptEditor)
        self.statusbar.setGeometry(QtCore.QRect(0,425,624,24))
        self.statusbar.setObjectName("statusbar")
        ScriptEditor.setStatusBar(self.statusbar)
        self.actionClose = QtGui.QAction(ScriptEditor)
        self.actionClose.setObjectName("actionClose")
        self.actionExit = QtGui.QAction(ScriptEditor)
        self.actionExit.setObjectName("actionExit")
        self.actionRun = QtGui.QAction(ScriptEditor)
        self.actionRun.setObjectName("actionRun")
        self.actionRunConsole = QtGui.QAction(ScriptEditor)
        self.actionRunConsole.setObjectName("actionRunConsole")
        self.actionNewPython = QtGui.QAction(ScriptEditor)
        self.actionNewPython.setObjectName("actionNewPython")
        self.actionNewQtScript = QtGui.QAction(ScriptEditor)
        self.actionNewQtScript.setObjectName("actionNewQtScript")
        self.menu_New.addAction(self.actionNewPython)
        self.menu_New.addAction(self.actionNewQtScript)
        self.menuFile.addAction(self.menu_New.menuAction())
        self.menuFile.addAction(self.actionClose)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionExit)
        self.menuRun.addAction(self.actionRun)
        self.menuRun.addAction(self.actionRunConsole)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuRun.menuAction())

        self.retranslateUi(ScriptEditor)
        QtCore.QMetaObject.connectSlotsByName(ScriptEditor)

    def retranslateUi(self, ScriptEditor):
        ScriptEditor.setWindowTitle(QtGui.QApplication.translate("ScriptEditor", "Script Editor", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("ScriptEditor", "&File", None, QtGui.QApplication.UnicodeUTF8))
        self.menu_New.setTitle(QtGui.QApplication.translate("ScriptEditor", "&New", None, QtGui.QApplication.UnicodeUTF8))
        self.menuRun.setTitle(QtGui.QApplication.translate("ScriptEditor", "&Run", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setText(QtGui.QApplication.translate("ScriptEditor", "&Close", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+W", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("ScriptEditor", "&Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRun.setText(QtGui.QApplication.translate("ScriptEditor", "&Run", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRun.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+R", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRunConsole.setText(QtGui.QApplication.translate("ScriptEditor", "Run script in &console", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRunConsole.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+C", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNewPython.setText(QtGui.QApplication.translate("ScriptEditor", "Python", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNewPython.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNewQtScript.setText(QtGui.QApplication.translate("ScriptEditor", "QtScript", None, QtGui.QApplication.UnicodeUTF8))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    ScriptEditor = QtGui.QMainWindow()
    ui = Ui_ScriptEditor()
    ui.setupUi(ScriptEditor)
    ScriptEditor.show()
    sys.exit(app.exec_())

