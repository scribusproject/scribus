import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
//import QtQuick.Layouts 1.0

/*
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
    */
Rectangle {
    width: 300
    height: 200
    Text {
      //  text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        onClicked: {
            Qt.close();
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
            x: 5
            y: 5
            text: qsTr("X:")
            font.pixelSize: 10
        }
        SpinBox {
            id: xSpinBox
            objectName: "xSpinBox"
            x: 25
            y: 3
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
            x: 5
            y: 25
            text: qsTr("Y:")
            font.pixelSize: 10
        }
        SpinBox {
            id: ySpinBox
            objectName: "ySpinBox"
            x: 25
            y: 23
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
            x: 105
            y: 5
            text: qsTr("W:")
            font.pixelSize: 10
        }
        SpinBox {
            id: wSpinBox
            objectName: "wSpinBox"
            x: 125
            y: 3
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
            x: 105
            y: 25
            text: qsTr("H:")
            font.pixelSize: 10
        }
        SpinBox {
            id: hSpinBox
            objectName: "hSpinBox"
            x: 125
            y: 23
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

    CheckBox {
        x: 204
        y: 110
        objectName: "closeCheckBox"

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
                text: "Close"
                font.pixelSize: 10
                font.italic: true
                color: "red"
            }
        }
    }
}
