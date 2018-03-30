import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: curLabel
    height: 50
    function backLight(){
    if(is_found)
        if(is_selected)
            return "red"
        else
            return "yellow"
    else
        if(is_selected)
            return "gray"
        else
            return "lightgray"
    }

    Rectangle {
        id: rectangle
        anchors.fill: parent
        anchors.margins: 2
        color: backLight()
        radius: 5
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5


        RowLayout {
            id: rowLayout
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Text {
                id: val_label
                text: value
                visible: is_found
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: unit_label
                text: unit
                visible: is_found
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: tim_label
                text: time
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

        }

        MouseArea{
            height: parent.height
            width: parent.width
            onClicked: is_selected = !is_selected;

        }

    }
}
