import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2
import PlatformSet 1.0

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    property bool valueChanged: false

    signal pressPos(int x,int y,int w,int h)

    onPressPos: {
        if(x+keyboardvir.width > parent.width){
            keyboardvir.x = parent.width - keyboardvir.width
        }else{
            keyboardvir.x = x
        }
        if(y + h + keyboardvir.height > parent.height){
            keyboardvir.y = y - keyboardvir.height
        }else{
            keyboardvir.y = y+h
        }

        keyboardvir.visible = true;

    }

    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onClicked: {
            keyboardvir.visible = false

        }
    }

    Connections{
        target: bubiaoconfig
        onPressed:{
            if(setindex == index){

                console.log("IC设置")

            }
        }
    }

    Label{
        id:icmainserverip
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
//        width: labelwidth
        text: qsTr("IC卡认证主服务器")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:icmainserveriptext
        width: boxwidth
        anchors.verticalCenter: icmainserverip.verticalCenter
        anchors.left: icmainserverip.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:icsecserverip
        anchors.top: icmainserverip.bottom
        anchors.topMargin: 10
        anchors.left: icmainserverip.left
//        width: labelwidth
        text: qsTr("IC卡认证副服务器")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:icsecserveriptext
        width: boxwidth
        anchors.verticalCenter: icsecserverip.verticalCenter
        anchors.left: icsecserverip.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:ictcpport
        anchors.top: icsecserverip.bottom
        anchors.topMargin: 10
        anchors.left: icsecserverip.left
//        width: labelwidth
        text: qsTr("TCP端口")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:ictcpporttext
        width: boxwidth
        anchors.verticalCenter: ictcpport.verticalCenter
        anchors.left: ictcpport.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:icudpport
        anchors.top: ictcpport.bottom
        anchors.topMargin: 10
        anchors.left: ictcpport.left
//        width: labelwidth
        text: qsTr("UDP端口")
        font.pixelSize: fontpixelSize

    }

    TextField{
        id:icudpporttext
        width: boxwidth
        anchors.verticalCenter: icudpport.verticalCenter
        anchors.left: icudpport.right
        anchors.leftMargin: 10
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    InputPanel {
        id: keyboardvir
        visible: false
//            anchors.right: parent.right
//            anchors.left: parent.left
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottom: parent.bottom
        width: parent.width/1.5
//        height: parent.height/3
        onActiveChanged: {
            if(!active) { visible = false; }
        }
    }



}
