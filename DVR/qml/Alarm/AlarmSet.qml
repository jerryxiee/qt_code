import QtQuick 2.0
import QtQuick.Controls 2.2
import "../delegate"

Rectangle {
    id:alarmsetroot

    property bool press: false
    signal suresignal
    signal cancelsignal

//    Row{
        Rectangle {
            id: setSelect
            width:alarmsetroot.width/5
            height:alarmsetroot.height - button.height
            color: "#DDDDDD"

            ListModel {
                id: pageModel
                ListElement {
                    title: "IO报警配置"
                    page: "IOAlarm/IOAlarm.qml"
                    source:""
                }
                ListElement {
                    title: "录像报警配置"
                    page: "RecordAlarm/RecordAlarm.qml"
                    source:""
                }
                ListElement {
                    title: "异常报警配置"
                    page: "ExceAlarm/ExceAlarm.qml"
                    source:""
                }

            }
          ScrollView {
            anchors.fill: parent
                ListView {
                    id:listview
            //                keyNavigationWraps:true
                    model: pageModel
                    clip: true
                    highlight: Rectangle { color: "#00CED1" }
                    anchors.fill: parent
                    delegate: SetDelegate1 {
                        height:40
                        text: title
//                        imageSource: source
        //                backgroundcolor: ListView.isCurrentItem?"#157efb":"#53d769"
                        onClicked: {
                            ListView.view.currentIndex = index
                            console.log(index)
                            console.log(Qt.resolvedUrl(page))
        //                    stackview.push(Qt.resolvedUrl(page))
                            load.source = page
                        }
                    }
                }
            }

        }

        Rectangle {
            id:setAttr
            width:alarmsetroot.width - setSelect.width
            height:alarmsetroot.height - button.height
            anchors.left: setSelect.right
            color:"lightBlue"

            Loader{
                id:load
                anchors.fill: setAttr
                source: "IOAlarm/IOAlarm.qml"
            }

        }
//    }

    Rectangle{
        id:button
        width: alarmsetroot.width
        height: 40
        color: "#BBBBBB"
        anchors.bottom: alarmsetroot.bottom

        Button{
            id:cancel
            width:120
            height:40
//            anchors.bottom:record.bottom
            anchors.rightMargin:20
            anchors.right:button.right
//            anchors.bottomMargin:40
            text:qsTr("取消")

            onClicked:{
                  cancelsignal()
                stackview.pop()
            }
        }

        Button{
            id:sure
            width:120
            height:40
//            anchors.bottom:record.bottom
            anchors.rightMargin:10
            anchors.right:cancel.left
//            anchors.bottomMargin:40
            text:qsTr("应用")

            onClicked:{
                suresignal()
            }
        }


    }


}
