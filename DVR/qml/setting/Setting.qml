import QtQuick 2.0
import QtQuick.Controls 1.2
import "../delegate"

Item {
    id:setting
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
//                keyNavigationWraps:true
        model: pageModel
        anchors.fill: parent
        delegate: SetDelegate {
            height:50
            text: title
            onClicked: {
                console.log(Qt.resolvedUrl(page))
                console.log("width "+setting.width)
                console.log("height "+setting.height)
                stackview.push(Qt.resolvedUrl(page))
            }
        }
    }

//    StackView {
//        id: setstackView
//        anchors.fill: parent
//        // Implements back key navigation
//        focus: true
//        Keys.onReleased: if (event.key === Qt.Key_Back && setstackView.depth > 1) {
//                             setstackView.pop();
//                             event.accepted = true;
//                         }

//        initialItem: Item {
//            width: parent.width
//            height: parent.height
//            ListView {
////                keyNavigationWraps:true
//                model: pageModel
//                anchors.fill: parent
//                delegate: SetDelegate {
//                    text: title
//                    onClicked: {
//                        console.log(Qt.resolvedUrl(page))
//                        setstackView.push(Qt.resolvedUrl(page))
//                    }
//                }
//            }
//        }
//    }

}
