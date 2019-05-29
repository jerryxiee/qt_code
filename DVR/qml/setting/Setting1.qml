import QtQuick 2.0

Rectangle {
    id:setparent

    property bool press: false

    Item {
        id: setSelect
        width:setparent.width/5
        height:setparent.height

        ListModel {
            id: pageModel
            ListElement {
                title: "录像配置"
                page: "content/RecordSet.qml"
            }
            ListElement {
                title: "通道配置"
                page: "content/SliderPage.qml"
            }
            ListElement {
                title: "网络配置"
                page: "content/ProgressBarPage.qml"
            }

        }

        ListView {
            id:listview
    //                keyNavigationWraps:true
            model: pageModel
            clip: true
            highlight: Rectangle { color: "#00CED1" }
            anchors.fill: parent
            delegate: SetDelegate1 {
                height:50
                text: title
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

    Item {
        id:setAttr
        width:setparent.width - setSelect.width
        height:setparent.height
        anchors.right: setparent.right

        Loader{
            id:load
            anchors.fill: parent
        }

    }

}
