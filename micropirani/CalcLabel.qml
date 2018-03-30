import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: calcLabel
    height: 50

    function backLight(){
    if(is_found)
        return "yellow"
    else
        return "lightgray"
    }

    Rectangle {
        id: rectangle_calc
        anchors.fill: parent
        anchors.margins: 2
        color: backLight()
        radius: 5
        anchors.rightMargin: 5
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5

        RowLayout {
            id: rowLayoutCalc
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Text {
                text: index
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: calc_val_label
                text: value
                visible: is_found
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: calc_unit_label
                text: unit
                visible: is_found
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: calc_tim_label
                text: time
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }

            Text{
            width:20
            height:20
            text:"x"
            MouseArea{
                width: parent.width
                height: parent.height
            onClicked: calcModel.remove(index)
            }
            }

        }

    }
}
