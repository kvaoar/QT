import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import my.cursorstorage 1.0

Item {
    id: curList
/*
    function getSelected(model){
        console.log("count = "+model.rowCount());
        for( var i = 0; i < model.rowCount(); ++i){
            var c = dataModel[i]
            console.log("c = "+c)
            if (c.is_selected)
            {
                            console.log("i = "+i);
                calcModel.append( {value: calcModel.count, unit: "mTorr", is_found: true})
            }
        }
    }
*/
    ListModel {
        id: calcModel
    }

Rectangle{
    width: parent.width
    height: parent.height

        ToolBar {
            id: toolBar
            z: 2

            RowLayout {
                id: rowLayout
                width: 100
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top

                ToolButton {
                    id: toolButtonDelta
                    width: 30
                    height: 30
                    text: qsTr("delta")
                    onClicked: {
                        getSelected(dataModel)
                    }
                }

                ToolButton {
                    id: toolButton1
                    width: 30
                    height: 30
                    text: qsTr("-")
                    onClicked: calcModel.remove(dataModel.count-1)
                }
            }
        }

        Rectangle{
            y: 46
            z:1
            height: parent.height/2
            width: parent.width
            color: "#729fcf"
            ListView {
                id: listView
                clip:true
                height: parent.height
                width: parent.width
                spacing: 5
                model: dataModel

                delegate: CurLabel {
                    height: 40
                    width: listView.width
                }


                add: Transition {
                    NumberAnimation {
                        property: "scale"
                        easing.amplitude: 0.1
                        easing.type: Easing.OutCirc
                        from: 0
                        to: 1.0
                        duration: 300
                    }
                }

                remove: Transition {
                    NumberAnimation {
                        property: "scale"
                        easing.type: Easing.InCirc
                        from: 1.0
                        to: 0.0
                        duration: 300
                    }
                }
            }
        }


        Rectangle{
            y: parent.height/2+toolBar.height
            height: parent.height - parent.height/2-toolBar.height
            width: parent.width
            color: "#8ae234"
                ListView{
                    id: calcView
                    clip:true
                    height: parent.height
                    width: parent.width
                    spacing: 5
                    model:calcModel
                delegate: CalcLabel{
                    height: 40
                    width: listView.width
                    }
                }
        }


    }

}
