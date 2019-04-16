import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 2.0

Rectangle {
    id:mainwidow
    color: "blue"
   // width: 800;height: 600

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
