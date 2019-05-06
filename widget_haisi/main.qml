import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id:mainwidow
    visible: true
    color: "black"
    width: 1280;height: 720
//    opacity: 0.5


    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onClicked:{
            if(mouse.button === Qt.LeftButton){
                console.log("left")
            }else if(mouse.button === Qt.RightButton){
                console.log("right")
            }else{
                console.log("mid")
            }

        }
    }


//    Image {
//        id: back
//        anchors.fill: parent
//        source: "images/background.png"
//    }

    Rectangle{
        id:rect
        width: mainwidow.width
        height: 48
        color: "black"
        Row{
            Button{
                id:backbutton
                anchors.left : rect.left

            }

            Text {
                id: maintext
                color: "white"
                font.pixelSize: 32
                text: qsTr("主菜单")
                anchors.centerIn: rect.Center
            }

            Button{
                id:homebutton
                anchors.right: rect.right

            }
        }
    }
//    Button{
//        id:button
//        anchors.centerIn: parent
//        text: "button"
//        onClicked: {
////            mainwidow.opacity -= 0.1
////            if(mainwidow.opacity < 0)
////                mainwidow.opacity = 1
//        }

//    }





}
