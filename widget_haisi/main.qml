import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 2.0

Rectangle {
    id:mainwidow
    color: "blue"
   // width: 800;height: 600


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
    Button{
        id:button
        anchors.centerIn: parent
        text: "button"
        onClicked: {
            opacity -= 0.1
            if(opacity < 0)
                opacity = 1
        }

    }



}
