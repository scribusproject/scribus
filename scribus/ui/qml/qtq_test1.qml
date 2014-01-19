/*
import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1
*/
import QtQuick 2.0

Rectangle {
	width: 360
	height: 360
	Text {
		anchors.centerIn: parent
		text: "Hello World"
	}
	MouseArea {
		anchors.fill: parent
		onClicked: {
			Qt.quit();
		}
	}
}



