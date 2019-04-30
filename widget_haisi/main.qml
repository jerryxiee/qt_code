import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id:mainwidow
    visible: true
//    color: "blue"
    width: 1280;height: 720



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

    Image {
        id: back
        anchors.fill: parent
        source: "images/background.png"
    }
    Button{
        id:button
        anchors.centerIn: parent
        text: "button"
        onClicked: {
            mainwidow.opacity -= 0.1
            if(mainwidow.opacity < 0)
                mainwidow.opacity = 1
        }

    }





}
