"""
Simple dialog which asks if you want to allow or deny execution of a script.
A list of possible problems is show. 
The analysis is done in safe_eval which is called from scripter_runtime
like this module. 

BTW, safe_eval is great module by Jeremy Sanders used inside Veusz.
It only allows a very small subset of Python which is considered safe.

XXX: Perhaps refactor some external functionality into this module
"""
from PyQt4.QtCore import pyqtSignature
from PyQt4.QtGui import QDialog, QApplication

from permitdlg_ui import Ui_PermitDialog



class PermitDialog(QDialog):


    def __init__(self, filename, problems):
        QDialog.__init__(self)
        self.ui = Ui_PermitDialog()
        self.ui.setupUi(self)
        # XXX: re-enable later, remember does currently not work currectly
        self.ui.rememberCheck.hide() 
        self.ui.filenameLabel.setText(filename)
        # XXX: only show specific reasons for found problems
        self.ui.problemsBrowser.setText("""
        <i>Please look into the source for further investigation.</i>
        <ul>
        %s
        </ul>
        <p>To ensure safe execution importing external modules is not allowed by default 
        because external modules could access your system directly.
        <br/>Additonally access to "private" attributes is not allowed because accessing 
        them could trigger side-effects which may help to break out of the sandbox.<br/>
        Unfortunately exceptions are also a security problem because they can change 
        the control flow and you could access the stack frame.</p>
        """ % "".join(["<li>%s</li>" % p for p in problems]))
        self.ui.problemsBrowser.hide()
        self.resize(self.width(), self.sizeHint().height())

    
    @pyqtSignature("")
    def on_allowButton_clicked(self):
        if self.ui.rememberCheck.isChecked():
            self.done(-1)
        else:
            self.done(1)

        
    @pyqtSignature("")
    def on_denyButton_clicked(self):
        if self.ui.rememberCheck.isChecked():
            self.done(-2)
        else:
            self.done(2)


    @pyqtSignature("")
    def on_detailsButton_clicked(self):
        self.ui.problemsBrowser.setVisible(self.ui.detailsButton.isChecked())
        self.resize(self.width(), self.sizeHint().height())



def ask(filename, problems):
    """
    Use this fuction
    """
    return PermitDialog(filename, problems).exec_()



if __name__ == "__main__":
    # Demo:
    import sys
    import safe_eval
    problems = safe_eval.checkCode(open("permitdlg.py").read())
    app = QApplication(sys.argv)
    print ask("permitdlg.py", problems)
