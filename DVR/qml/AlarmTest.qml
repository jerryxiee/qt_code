import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {

    Text {
        id: chn
        text: qsTr("channel")
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 20
        anchors.leftMargin: 50
    }

    ComboBox{
        id:chnindex
        model: ["0","1","2","3","4","5","6","7"]
        anchors.left: chn.right
        anchors.leftMargin:50
        anchors.verticalCenter: chn.verticalCenter
    }

    Button{
        id:move1
        text: qsTr("move enable")
        anchors.top: chn.bottom
        anchors.left: chn.left
        anchors.topMargin: 20

        onClicked: VideoAlarmTest.setMoveDetect(chnindex.currentIndex,true)
    }
    Button{
        id:move2
        text: qsTr("move disenable")
        anchors.left:move1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: move1.verticalCenter
        onClicked: VideoAlarmTest.setMoveDetect(chnindex.currentIndex,false)
    }

    Text {
        id: iotype
        text: qsTr("IOTYPE")
        anchors.top: move1.bottom
        anchors.topMargin: 20
        anchors.left: move1.left
    }

    ComboBox{
        id:iotypeindex
        model: ["0","1","2","3"]
        anchors.left: iotype.right
        anchors.leftMargin:50
        anchors.verticalCenter: iotype.verticalCenter
    }

    Button{
        id:io1
        text: qsTr("io enable")
        anchors.top: iotype.bottom
        anchors.topMargin: 20
        anchors.left: iotype.left

        onClicked: VideoAlarmTest.setIOAlarm(chnindex.currentIndex,iotypeindex.currentIndex,true)
    }
    Button{
        id:io2
        text: qsTr("io disenable")
        anchors.left:io1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: io1.verticalCenter
        onClicked: VideoAlarmTest.setIOAlarm(chnindex.currentIndex,iotypeindex.currentIndex,false)
    }

    Button{
        id:test
        anchors.top: io2.bottom
        anchors.topMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 100

        onClicked: console.log("x:"+x+" y:"+y)
    }

}
