import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0

ApplicationWindow {
    width: 360
    height: 360
    menuBar:MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "Open..." }
            MenuItem { text: "Close" }
        }

        Menu {
            title: "Edit"
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }
        }
    }
    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }

        CheckBox {
            x: 54
            y: 70
            text: "My new checkbox"
            style: CheckBoxStyle {
                indicator: Rectangle {
                        implicitWidth: 10
                        implicitHeight: 10
                        radius: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            visible: control.checked
                            color: "#555"
                            border.color: "#333"
                            radius: 1
                            anchors.margins: 2
                            anchors.fill: parent
                        }
                }
                label: Text {
                    text: "Hello world"
                    font.pixelSize: 10
                    color: "black"
                }
            }
        }

        CheckBox {
            x: 204
            y: 70
            text: "My second checkbox"
            style: CheckBoxStyle {
                indicator: Rectangle {
                        implicitWidth: 10
                        implicitHeight: 10
                        radius: 1
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        border.width: 1
                        Rectangle {
                            visible: control.checked
                            color: "#555"
                            border.color: "#333"
                            radius: 1
                            anchors.margins: 2
                            anchors.fill: parent
                        }
                }
                label: Text {
                    text: "Hello world"
                    font.pixelSize: 10
                    color: "black"
                }
            }
        }

        Label {
            id: label1
            x: 20
            y: 20
            text: qsTr("X:")
            font.pixelSize: 10
        }
        SpinBox {
            id: xSpinBox
            x: 40
            y: 18
            font.pixelSize: 10
            decimals:2
            style: SpinBoxStyle{
                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 15
                    border.color: "gray"
                    radius: 2
                }
            }
        }

        Label {
            id: label2
            x: 20
            y: 40
            text: qsTr("Y:")
            font.pixelSize: 10
        }
        SpinBox {
            id: ySpinBox
            x: 40
            y: 38
            font.pixelSize: 10
            decimals:2
            style: SpinBoxStyle{
                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 15
                    border.color: "gray"
                    radius: 2
                }
            }
        }

        Label {
            id: labelW
            x: 120
            y: 40
            text: qsTr("W:")
            font.pixelSize: 10
        }
        SpinBox {
            id: wSpinBox
            x: 140
            y: 38
            font.pixelSize: 10
            decimals:2
            style: SpinBoxStyle{
                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 15
                    border.color: "gray"
                    radius: 2
                }
            }
        }

        Label {
            id: labelH
            x: 120
            y: 20
            text: qsTr("H:")
            font.pixelSize: 10
        }
        SpinBox {
            id: hSpinBox
            x: 140
            y: 18
            font.pixelSize: 10
            decimals:2
            style: SpinBoxStyle{
                background: Rectangle {
                    implicitWidth: 60
                    implicitHeight: 15
                    border.color: "gray"
                    radius: 2
                }
            }
        }


        ComboBox {
            id: comboBox1
            x: 54
            y: 105
            style: ComboBoxStyle{
                //background: Rectangle {
                //    implicitWidth: 60
                //    implicitHeight: 15
                //}
            }
        }

    }
}
