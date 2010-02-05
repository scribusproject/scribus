# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'excepthook.ui'
#
# Created: Sun Jun 15 01:19:42 2008
#      by: PyQt4 UI code generator 4.4.2
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ExceptHookDialog(object):
    def setupUi(self, ExceptHookDialog):
        ExceptHookDialog.setObjectName("ExceptHookDialog")
        ExceptHookDialog.resize(542,290)
        self.verticalLayout = QtGui.QVBoxLayout(ExceptHookDialog)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setSpacing(10)
        self.gridLayout.setObjectName("gridLayout")
        self.label = QtGui.QLabel(ExceptHookDialog)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label,0,0,1,1)
        self.closeButton = QtGui.QPushButton(ExceptHookDialog)
        self.closeButton.setObjectName("closeButton")
        self.gridLayout.addWidget(self.closeButton,0,1,1,1)
        self.exceptionLabel = QtGui.QLabel(ExceptHookDialog)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.exceptionLabel.setFont(font)
        self.exceptionLabel.setObjectName("exceptionLabel")
        self.gridLayout.addWidget(self.exceptionLabel,1,0,1,1)
        self.detailsButton = QtGui.QPushButton(ExceptHookDialog)
        self.detailsButton.setObjectName("detailsButton")
        self.gridLayout.addWidget(self.detailsButton,1,1,1,1)
        self.verticalLayout.addLayout(self.gridLayout)
        self.tracebackBrowser = QtGui.QTextBrowser(ExceptHookDialog)
        self.tracebackBrowser.setMinimumSize(QtCore.QSize(0,200))
        self.tracebackBrowser.setObjectName("tracebackBrowser")
        self.verticalLayout.addWidget(self.tracebackBrowser)

        self.retranslateUi(ExceptHookDialog)
        QtCore.QMetaObject.connectSlotsByName(ExceptHookDialog)

    def retranslateUi(self, ExceptHookDialog):
        ExceptHookDialog.setWindowTitle(QtGui.QApplication.translate("ExceptHookDialog", "Script error", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("ExceptHookDialog", "An exception occured while running the script.", None, QtGui.QApplication.UnicodeUTF8))
        self.closeButton.setText(QtGui.QApplication.translate("ExceptHookDialog", "&Close", None, QtGui.QApplication.UnicodeUTF8))
        self.exceptionLabel.setText(QtGui.QApplication.translate("ExceptHookDialog", "Exception", None, QtGui.QApplication.UnicodeUTF8))
        self.detailsButton.setText(QtGui.QApplication.translate("ExceptHookDialog", "&Details >>", None, QtGui.QApplication.UnicodeUTF8))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    ExceptHookDialog = QtGui.QDialog()
    ui = Ui_ExceptHookDialog()
    ui.setupUi(ExceptHookDialog)
    ExceptHookDialog.show()
    sys.exit(app.exec_())

