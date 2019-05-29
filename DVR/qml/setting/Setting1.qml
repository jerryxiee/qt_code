import QtQuick 2.0

Rectangle {
    id:setparent


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
            anchors.fill: parent
            delegate: SetDelegate1 {
                height:50
                text: title
                onClicked: {
                    backgroundcolor = "blue"
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
