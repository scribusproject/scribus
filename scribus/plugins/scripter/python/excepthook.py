"""
Exception hook
If some unexpected error occures it can be shown in a nice looking dialog.
Especially useful is the traceback view.

Things to extend: Clicking on the filename should open an editor.
Things to consider: Mail exceptions, copy to clipboard or send to bug tracker.
"""
import sys
import cgitb
import atexit

from PyQt4.QtCore import pyqtSignature, Qt
from PyQt4.QtGui import QDialog, QApplication

from excepthook_ui import Ui_ExceptHookDialog



def on_error(exc_type, exc_obj, exc_tb):
    """
    This is the callback function for sys.excepthook
    """
    dlg = ExceptHookDialog(exc_type, exc_obj, exc_tb)
    dlg.show()
    dlg.exec_()



def show_current_error(title=None):
    """
    Call this function to show the current error.
    It can be used inside an except-block.
    """
    dlg = ExceptHookDialog(sys.exc_type, sys.exc_value, sys.exc_traceback, title)
    dlg.show()
    dlg.exec_()


def install():
    "activates the error handler"
    sys.excepthook = on_error


    
def uninstall():
    "removes the error handler"
    sys.excepthook = sys.__excepthook__

atexit.register(uninstall)


class ExceptHookDialog(QDialog):


    def __init__(self, exc_type, exc_obj, exc_tb, title=None):
        QDialog.__init__(self)
        self.ui = Ui_ExceptHookDialog()
        self.ui.setupUi(self)
        if title:
            self.setWindowTitle(self.windowTitle() + ": " + title)
        self.ui.detailsButton.setCheckable(True)
        self.setExtension(self.ui.tracebackBrowser)
        self.setOrientation(Qt.Vertical)
        msg = "%s: %s" % (exc_type.__name__, exc_obj)
        self.ui.exceptionLabel.setText(msg)
        html = cgitb.html((exc_type, exc_obj, exc_tb))
        self.ui.tracebackBrowser.setText(html)
        self.resize(self.sizeHint())


    @pyqtSignature("")
    def on_closeButton_clicked(self):
        self.close()


    @pyqtSignature("")
    def on_detailsButton_clicked(self):
        self.showExtension(self.ui.detailsButton.isChecked())



if __name__ == "__main__":
    # Some tests:
    app = QApplication(sys.argv)
    install()
    print "Triggering error 1"
    try:
        fail = 1 / 0
    except:
        show_current_error("Using inside except")
    print "Triggering error 2"
    fail2 = 1 / 0
    print "This will never be reached because excepthook"
    print "complains about fail2"
