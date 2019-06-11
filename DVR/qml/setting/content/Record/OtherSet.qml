import QtQuick 2.0
import QtQuick.Controls 1.2

Item {

    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: record
        onPressed:{
            if(setindex == index){

            }
        }
    }

    Text{
        id:timesetchn
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        text:qsTr("通道选择")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:timesetchnBox
        width: boxwidth
        anchors.left: timesetchn.right
        anchors.leftMargin: 100
        anchors.top: timesetchn.top
        height:timesetchn.height
        model: ["模拟通道0","模拟通道1","模拟通道2","模拟通道3",
            "模拟通道4","模拟通道5","模拟通道6","模拟通道7"]
    }

    Text{
        id:pretime
        anchors.top: timesetchn.bottom
        anchors.topMargin: 20
        anchors.left: timesetchn.left
        text:qsTr("预录时间")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:pretimeBox
        width: boxwidth
        anchors.left: pretime.right
        anchors.leftMargin: 100
        anchors.top: pretime.top
        height:pretime.height
        model: ["0","1s","5s","10s","30s",
            "60s"]
    }

    Text{
        id:deltime
        anchors.top: pretime.bottom
        anchors.topMargin: 20
        anchors.left: pretime.left
        text:qsTr("延时时间")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:deltimeBox
        width: boxwidth
        anchors.left: deltime.right
        anchors.leftMargin: 100
        anchors.top: deltime.top
        height:deltime.height
        model: ["0","1s","5s","10s","30s",
            "60s"]
    }
}
