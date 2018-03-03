import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: curLabel
    Row {
        id: row1
        Rectangle {
            width: 40
            height: 40
            color: colorCode
        }
        
        Text {
            text: name
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
        }
        spacing: 10
    }
}
