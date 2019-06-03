import QtQuick 2.0
import QtQuick.Controls 2.2
import "../delegate"

Rectangle {
    id:setparent

    property bool press: false
    signal suresignal
    signal cancelsignal

//    Row{
        Rectangle {
            id: setSelect
            width:setparent.width/5
            height:setparent.height - button.height
            color: "#DDDDDD"

            ListModel {
                id: pageModel
                ListElement {
                    title: "录像配置"
                    page: "content/Record/RecordSet.qml"
                    source:"qrc:/qml/setting/images/settings.png"
                }
                ListElement {
                    title: "通道配置"
                    page: "content/ChnProperty/ChannelSet.qml"
                    source:""
                }
                ListElement {
                    title: "网络配置"
                    page: "content/ProgressBarPage.qml"
                    source:""
                }
                ListElement {
                    title: "系统时间配置"
                    page: "content/ProgressBarPage.qml"
                    source:""
                }
                ListElement {
                    title: "预览配置"
                    page: "content/ProgressBarPage.qml"
                    source:""
                }
                ListElement {
                    title: "报警配置"
                    page: "content/ProgressBarPage.qml"
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
            width:setparent.width - setSelect.width
            height:setparent.height - button.height
            anchors.left: setSelect.right

            Loader{
                id:load
                anchors.fill: setAttr
                source: "content/Record/RecordSet.qml"
            }

        }
//    }

    Rectangle{
        id:button
        width: setparent.width
        height: 40
        color: "#BBBBBB"
        anchors.bottom: setparent.bottom

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
//                load.item.set()
                suresignal()
//                VideoControl.setVencAttr(setchnBox.currentText,setStreamBox.currentText,setStreamTypeBox.currentText,resolutionBox.currentText
//                                         ,bitRateTyprBox.currentText,imageQualityBox.currentText,frameRateBox.currentText,bitRateBox.currentText)
            }
        }


    }


}
