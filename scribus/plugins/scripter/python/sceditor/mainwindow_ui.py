# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created: Fri Aug 19 22:47:33 2011
#      by: PyQt4 UI code generator 4.8.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_ScriptEditor(object):
    def setupUi(self, ScriptEditor):
        ScriptEditor.setObjectName(_fromUtf8("ScriptEditor"))
        ScriptEditor.resize(624, 449)
        self.centralwidget = QtGui.QWidget(ScriptEditor)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        ScriptEditor.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(ScriptEditor)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 624, 25))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8("menuFile"))
        self.menu_New = QtGui.QMenu(self.menuFile)
        self.menu_New.setObjectName(_fromUtf8("menu_New"))
        self.menuRun = QtGui.QMenu(self.menubar)
        self.menuRun.setObjectName(_fromUtf8("menuRun"))
        ScriptEditor.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(ScriptEditor)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        ScriptEditor.setStatusBar(self.statusbar)
        self.actionClose = QtGui.QAction(ScriptEditor)
        self.actionClose.setObjectName(_fromUtf8("actionClose"))
        self.actionExit = QtGui.QAction(ScriptEditor)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.actionRun = QtGui.QAction(ScriptEditor)
        self.actionRun.setObjectName(_fromUtf8("actionRun"))
        self.actionRunConsole = QtGui.QAction(ScriptEditor)
        self.actionRunConsole.setObjectName(_fromUtf8("actionRunConsole"))
        self.actionNewPython = QtGui.QAction(ScriptEditor)
        self.actionNewPython.setObjectName(_fromUtf8("actionNewPython"))
        self.actionNewQtScript = QtGui.QAction(ScriptEditor)
        self.actionNewQtScript.setObjectName(_fromUtf8("actionNewQtScript"))
        self.actionClear = QtGui.QAction(ScriptEditor)
        self.actionClear.setObjectName(_fromUtf8("actionClear"))
        self.actionSave_As = QtGui.QAction(ScriptEditor)
        self.actionSave_As.setObjectName(_fromUtf8("actionSave_As"))
        self.actionOpen = QtGui.QAction(ScriptEditor)
        self.actionOpen.setObjectName(_fromUtf8("actionOpen"))
        self.actionSave = QtGui.QAction(ScriptEditor)
        self.actionSave.setObjectName(_fromUtf8("actionSave"))
        self.menu_New.addAction(self.actionNewPython)
        self.menu_New.addAction(self.actionNewQtScript)
        self.menuFile.addAction(self.menu_New.menuAction())
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_As)
        self.menuFile.addAction(self.actionClose)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionExit)
        self.menuRun.addAction(self.actionRun)
        self.menuRun.addAction(self.actionRunConsole)
        self.menuRun.addAction(self.actionClear)
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
        self.actionClear.setText(QtGui.QApplication.translate("ScriptEditor", "Clear", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClear.setToolTip(QtGui.QApplication.translate("ScriptEditor", "Clear The Console", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_As.setText(QtGui.QApplication.translate("ScriptEditor", "Save &As", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_As.setToolTip(QtGui.QApplication.translate("ScriptEditor", "Save the  script", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_As.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+A", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("ScriptEditor", "&Open", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setToolTip(QtGui.QApplication.translate("ScriptEditor", "Open a script", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+O", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setText(QtGui.QApplication.translate("ScriptEditor", "&Save", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setToolTip(QtGui.QApplication.translate("ScriptEditor", "Save the current script", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setShortcut(QtGui.QApplication.translate("ScriptEditor", "Ctrl+S", None, QtGui.QApplication.UnicodeUTF8))

