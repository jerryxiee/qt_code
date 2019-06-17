import QtQuick 2.0
import QtQuick.Controls 1.2
import "../../../Controls"

Item {
    property int fontpixelSize : 32
    property int boxwidth: 150
    property int index: 0

    Connections{
        target: preview
        onPressed:{
            if(setindex == index){
                console.log("preview normal")
            }
        }
    }

    Text {
        id: porttype
        width: boxwidth
        text: qsTr("输出端口")
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        font.pixelSize:fontpixelSize

    }
    ComboBox{
        id:portBox
        width: boxwidth
        height: porttype.height
        anchors.left: porttype.right
        anchors.leftMargin: 100
        anchors.verticalCenter: porttype.verticalCenter
        model: ["VGA","HDMI","CVBS"]
    }

    Text {
        id: previewtype
        width: boxwidth
        text: qsTr("预览模式")
        anchors.top: porttype.bottom
        anchors.topMargin: 20
        anchors.left: porttype.left
        font.pixelSize:fontpixelSize

    }
    ComboBox{
        id:previewtypeBox
        width: boxwidth
        height: previewtype.height
        anchors.left: previewtype.right
        anchors.leftMargin: 100
        anchors.verticalCenter: previewtype.verticalCenter
        model: ["9画面","4画面","1画面"]
    }

    Text {
        id: swaptime
        width: boxwidth
        text: qsTr("切换时间")
        anchors.top: previewtype.bottom
        anchors.topMargin: 20
        anchors.left: previewtype.left
        font.pixelSize:fontpixelSize

    }
    ComboBox{
        id:swaptimeBox
        width: boxwidth
        height: swaptime.height
        anchors.left: swaptime.right
        anchors.leftMargin: 100
        anchors.verticalCenter: swaptime.verticalCenter
        model: ["不切换","30s","60s"]
    }

    MyCheckBox{
        id:netauto
        anchors.top: swaptime.bottom
        anchors.topMargin: 20
        anchors.left: swaptime.left
        pixSize:fontpixelSize

        str:qsTr("音频输出")

    }

}
