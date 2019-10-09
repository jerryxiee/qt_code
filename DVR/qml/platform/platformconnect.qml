import QtQuick 2.0
import QtQuick.Controls 2.2
import "../delegate"
import QtQuick.VirtualKeyboard 2.2

Rectangle {
    id:platformroot

    property bool press: false
    signal suresignal
    signal cancelsignal

//    Row{
        Rectangle {
            id: setSelect
            width:platformroot.width/5
            height:platformroot.height - button.height
            color: "#DDDDDD"

            ListModel {
                id: pageModel
                ListElement {
                    title: "部标注册"
                    page: "bubiao/bubiaoregister.qml"
                    source:""
                }
                ListElement {
                    title: "部标参数"
                    page: "bubiao/bubiaoConfig.qml"
                    source:""
                }
                ListElement {
                    title: "调度设置"
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
            width:platformroot.width - setSelect.width
            height:platformroot.height - button.height
            anchors.left: setSelect.right
            color:"lightBlue"

            Loader{
                id:load
                anchors.fill: setAttr
                source: "bubiao/bubiaoregister.qml"
            }

        }
//    }

    Rectangle{
        id:button
        width: platformroot.width
        height: 40
        color: "#BBBBBB"
        anchors.bottom: platformroot.bottom

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

//    InputPanel {
//        id: keyboardvir
//        visible: false
////            anchors.right: parent.right
////            anchors.left: parent.left
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
//        width: parent.width /2
//        height: parent.height/4
//        onActiveChanged: {
//            if(!active) { visible = false; }
//        }
//    }

}
