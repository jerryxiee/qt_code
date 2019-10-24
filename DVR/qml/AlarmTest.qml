import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2
import "Controls"

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

        onClicked: VideoTest.setMoveDetect(chnindex.currentIndex,true)
    }
    Button{
        id:move2
        text: qsTr("move disenable")
        anchors.left:move1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: move1.verticalCenter
        onClicked: VideoTest.setMoveDetect(chnindex.currentIndex,false)
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

        onClicked: VideoTest.setIOAlarm(chnindex.currentIndex,iotypeindex.currentIndex,true)
    }
    Button{
        id:io2
        text: qsTr("io disenable")
        anchors.left:io1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: io1.verticalCenter
        onClicked: VideoTest.setIOAlarm(chnindex.currentIndex,iotypeindex.currentIndex,false)
    }

    Button{
        id:msgtest
        text: qsTr("msgtest")
        anchors.left: io2.left
        anchors.top:io2.bottom
        anchors.topMargin: 20

        onClicked: VideoTest.sentSeg(0);
    }

    Button{
        id:createsession
        text: qsTr("createsession")
        anchors.left: parent.left
        anchors.top:msgtest.bottom
        anchors.topMargin: 20

        onClicked: VideoTest.createSession(comb.currentIndex,check.checked)
    }

    ComboBox{
        id:comb
        model: [0,1,2,3,4,5,6,7]
        anchors.left: createsession.right
        anchors.leftMargin: 20
        anchors.verticalCenter: createsession.verticalCenter
    }
    CheckBox{
        id:check
        text: qsTr("直播")
        anchors.left: comb.right
        anchors.leftMargin: 20
        anchors.verticalCenter: createsession.verticalCenter
    }

    Button{
        id:deletesession
        text: qsTr("deletesession")
        anchors.left: check.right
        anchors.leftMargin: 20
        anchors.verticalCenter: createsession.verticalCenter

        onClicked: VideoTest.deleteSession(comb.currentIndex,check.checked);
    }

    Button{
        id:stopsession
        text: qsTr("stopsession")
        anchors.left: parent.left
        anchors.top:createsession.bottom
        anchors.topMargin: 20

        onClicked: VideoTest.stopSession(comb.currentIndex,check.checked)
    }

    Button{
        id:playsession
        text: qsTr("playsession")
        anchors.left: stopsession.right
        anchors.leftMargin: 20
        anchors.verticalCenter: stopsession.verticalCenter

        onClicked: VideoTest.playSession(comb.currentIndex,check.checked)
    }



//    Button{
//        id:test
//        anchors.top: io2.bottom
//        anchors.topMargin: 100
//        anchors.left: parent.left
//        anchors.leftMargin: 100

//        onClicked: {
//            videoTestSignal()
//            console.log("x:"+x+" y:"+y)
//        }
//    }

    Rectangle{
        id:videoplaytest
        width: 400
        height: parent.height
        color: "lightblue"
        anchors.top: parent.top
        anchors.right: parent.right

        Text {
            id: title
            text: qsTr("VideoPlayTest")
            anchors.top: parent.top
            anchors.horizontalCenter:parent.horizontalCenter
        }

        Button{
            id:play
            text: qsTr("search play")
            anchors.top: parent.top
            anchors.left: parent.left

            onClicked: {
                var num = 0;
                var list;
                for(var i=0;i<repeater.count;i++){
                    list = repeater.itemAt(i).children
                    if(list[0].checked){
                        num++;
                    }
                }
                console.log("checked num:"+num)
                if(num > 0)
                    VideoTest.setWinNum(num);

                for(var i=0;i<repeater.count;i++ ){
                    list = repeater.itemAt(i).children
                    if(list[0].checked){
                        VideoTest.play(i,list[1].text+" "+list[2].text,
                                          list[3].text+" "+list[4].text)
                    }
                }

                if(num > 0){
                    videoTestSignal()
                }

//                VideoTest.play(0,list[1].text+" "+list[2].text,
//                           list[3].text+" "+list[4].text)

//                videoTestSignal()
            }

        }

        Column{
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
//            spacing: 10
            Repeater{
                id:repeater
                model: 8

                Rectangle{
                    width:videoplaytest.width
                    height: 80
                    color: "lightblue"

                    CheckBox {
                        id: name
                        text: "channel"+index

                    }

                    MyDateTime{
                        id:startd
                        anchors.left: name.right
                        anchors.leftMargin: 20
                        width: 120
    //                        height: starttime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd")

    //                        onPressed: keyboardvir.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                    MyDateTime{
                        id:startt
                        anchors.verticalCenter: startd.verticalCenter
                        anchors.left: startd.right
                        anchors.leftMargin: 5
                        type: "time"
                        width: 120
    //                        height: starttime.height + 5
                        text: qsTr("00:00:00")

    //                        onPressed: keyboardvir.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                    MyDateTime{
                        id:endd
                        anchors.top: startd.bottom
                        anchors.topMargin: 2
                        anchors.left: startd.left
                        width: 120
    //                        height: starttime.height + 5
                        text: (new Date()).toLocaleString(Qt.locale(), "yyyy/MM/dd")

    //                        onPressed: keyboardvir.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                    MyDateTime{
                        id:endt
                        anchors.verticalCenter: endd.verticalCenter
                        anchors.left: endd.right
                        anchors.leftMargin: 5
                        type: "time"
                        width: 120
    //                        height: starttime.height + 5
                        text: qsTr("23:59:59")

    //                        onPressed: keyboardvir.visible = true;

                        onValueChanged: console.log("test value:"+value)
                    }

                }
            }
        }

    }

    InputPanel {
        id: keyboardvir
        visible: false
//            anchors.right: parent.right
//            anchors.left: parent.left
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width /2
        height: parent.height/4
        onActiveChanged: {
            if(!active) { visible = false; }
        }
    }

}
