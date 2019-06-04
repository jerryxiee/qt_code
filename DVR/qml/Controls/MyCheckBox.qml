import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2


Rectangle {

    property string str:""
    property int pixSize:25
    property alias checked: ll.checked
    property alias exclusiveGroup: ll.exclusiveGroup
    property alias checkedState:ll.checkedState//0未选中，2选中
    signal clicked;


    width: (ll.width /*+ str.length*/)
    height: pixSize

    CheckBox
    {
        id:ll
        anchors.fill: parent
        style:CheckBoxStyle{
//            indicator:Rectangle{
//                id:functChose
//                implicitWidth: langrageSize
//                implicitHeight: langrageSize
////                radius: langrageSize
//                border.color: control.activeFocus ? "darkblue" : "gray"
//                border.width: 2
////                Rectangle {
////                    visible: control.checked
////                    color: "#555"
////                    border.color: "#333"
////                    radius: langrageSize
////                    anchors.margins: 4
////                    anchors.fill: parent
////                }
//            }
            label: Label{
                id:string
                text:str
                font.pixelSize: pixSize
            }
        }
    }
}
