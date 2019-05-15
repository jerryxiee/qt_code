import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3


Rectangle {
    visible: true
//    width: 800
//    height: 600
//    color: "blue"
//    opacity: 0.2

    signal hidqmlsignal()
    property string title

    MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
            onClicked: {
                if(mouse.button === Qt.LeftButton){
                    console.log("left")
                }else if(mouse.button === Qt.RightButton){
                    console.log("right")
                }else{
                    hidqmlsignal()
                }

            }

        }

    Rectangle{
        id:maintool
        width: parent.width
        height: 40

        NaviButton {
            id: backButton

    //        edge: Qt.TopEdge
            enabled: true
            imageSource: "qrc:/images/back.png"
            anchors.left: maintool.left
            imageheight: 40
            imagewidth: 40

            onClicked: {
                console.log("back start")
                stackview.pop()
                console.log("back end")
            }
        }

        Text {
            id: name
            text: stackview.depth > 1 ? title:qsTr("主菜单")
            anchors.centerIn: maintool
            font.pixelSize: 24
        }

        NaviButton {
            id: homeButton

    //        edge: Qt.TopEdge
            enabled: true
            imageSource: "qrc:/images/home.png"
            anchors.right: maintool.right
            imageheight: 40
            imagewidth: 40

            onClicked: {
                hidqmlsignal()
                stackview.pop(null)
            }
        }


    }

    StackView{
        id:stackview
        x:0;y:maintool.height
        width: parent.width
        height: parent.height - y
        initialItem: Launcher{
            onLauncher: {
                console.log(titlename.substring(0,6))
                if(titlename.substring(0,6) == "widget"){
                    console.log("widget")
                }else{
                    title = titlename
                    stackview.push(page)
                }
            }
        }
    }


}
