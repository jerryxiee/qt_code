import QtQuick 2.0
import QtQuick.Controls 2.2
import "../delegate"

Rectangle {
    id:stateroot

    property bool press: false
    signal suresignal
    signal cancelsignal

//    Row{
        Rectangle {
            id: setSelect
            width:stateroot.width/5
            height:stateroot.height - button.height
            color: "#DDDDDD"

            ListModel {
                id: pageModel
                ListElement {
                    title: "通道状态"
                    page: "ChnState/ChnState.qml"
                    source:""
                }
                ListElement {
                    title: "报警状态"
                    page: "AlarmState/AlarmState.qml"
                    source:""
                }
                ListElement {
                    title: "平台接入状态"
                    page: "PlatformState/PlatformState.qml"
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
            width:stateroot.width - setSelect.width
            height:stateroot.height - button.height
            anchors.left: setSelect.right
            color:"lightBlue"

            Loader{
                id:load
                anchors.fill: setAttr
                source: "ChnState/ChnState.qml"
            }

        }
//    }

    Rectangle{
        id:button
        width: stateroot.width
        height: 40
        color: "#BBBBBB"
        anchors.bottom: stateroot.bottom

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
