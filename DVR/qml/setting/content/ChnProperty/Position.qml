import QtQuick 2.0
import QtQuick.Controls 1.2
//import QtMultimedia 5.0
//import FrameProvider 1.0

Item {
    property int fontpixelSize : 24
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: channelset
        onPressed:{
            if(setindex == index){

            }
        }
    }

    Text{
        id:setchn
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 100
        text:qsTr("通道选择")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:setchnBox
        width: boxwidth
        anchors.left: setchn.right
        anchors.leftMargin: 100
        anchors.top: setchn.top
        height:setchn.height
        model: ["模拟通道0","模拟通道1","模拟通道2","模拟通道3",
            "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]
    }

//    Rectangle{
//        width: 960
//        height: 540
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.top: setchn.bottom
//        anchors.topMargin: 10;

//        FrameProvider{
//               objectName: "provider"
//               id: provider
//        }

//        VideoOutput {
//            id: video
//            objectName: "display"
//            source: provider
//            anchors.fill: parent
//        }

//        Component.onCompleted: {
//            console.log(provider.height)
//            console.log(provider.width)
//            provider.start()

//        }
//    }


}
