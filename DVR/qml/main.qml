import QtQuick 2.2
//import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "Controls"


Rectangle {
    visible: true
//    width: 800
//    height: 600
    color: "#212126"
//    opacity: 0.2

    signal hidqmlsignal()
    signal videoDispSignal(string filename)
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
        color: "#555555"

        NaviButton {
            id: backButton

    //        edge: Qt.TopEdge
            enabled: true
            imageSource: "qrc:/images/back.png"
            anchors.left: maintool.left
            imageheight: 40
            imagewidth: 40

            onClicked: {
                if(stackview.depth > 1){
                    stackview.pop()
                }else{
                    hidqmlsignal()
                }
            }
        }

        Text {
            id: name
            text: stackview.depth > 1 ? title:qsTr("主菜单")
            anchors.centerIn: maintool
            font.pixelSize: 24
            color:"white"
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
                console.log(flag)
                if(flag == "widget"){
                    console.log("widget")
                }else{
                    title = titlename
                    stackview.push(page)
                }
            }
        }
    }


}
