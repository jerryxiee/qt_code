import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.VirtualKeyboard 2.2

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

                console.log("驾驶设置")

            }
        }
    }

    Label{
        id:minresttime
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
        text: qsTr("最小休息时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:minresttimetext
        width: 100
        anchors.verticalCenter: minresttime.verticalCenter
        anchors.left: minresttime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec
        anchors.verticalCenter: minresttimetext.verticalCenter
        anchors.left: minresttimetext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:speedcontinuetime
        anchors.verticalCenter: minresttime.verticalCenter
        anchors.left: sec.right
        anchors.leftMargin: 10
        text: qsTr("超速持续时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:speedcontinuetimetext
        width: 100
        anchors.verticalCenter: minresttime.verticalCenter
        anchors.left: speedcontinuetime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec1
        anchors.verticalCenter: minresttimetext.verticalCenter
        anchors.left: speedcontinuetimetext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }


    Label{
        id:carstoptime
        anchors.top: minresttime.bottom
        anchors.topMargin: 10
        anchors.left: minresttime.left
        text: qsTr("最长停车时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:carstoptimetext
        width: 100
        anchors.verticalCenter: carstoptime.verticalCenter
        anchors.left: carstoptime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec2
        anchors.verticalCenter: carstoptime.verticalCenter
        anchors.left: carstoptimetext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:fenceradius
        anchors.verticalCenter: carstoptime.verticalCenter
        anchors.left: sec2.right
        anchors.leftMargin: 10
        text: qsTr("电子围栏半径")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:fenceradiustext
        width: 100
        anchors.verticalCenter: fenceradius.verticalCenter
        anchors.left: fenceradius.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec3
        anchors.verticalCenter: fenceradius.verticalCenter
        anchors.left: fenceradiustext.right
        anchors.leftMargin: 5
        text: qsTr("米")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:drivertime
        anchors.top: carstoptime.bottom
        anchors.topMargin: 10
        anchors.left: carstoptime.left
        text: qsTr("连续驾驶时间门限")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:drivertimetext
        width: 100
        anchors.verticalCenter: drivertime.verticalCenter
        anchors.left: drivertime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec4
        anchors.verticalCenter: drivertime.verticalCenter
        anchors.left: drivertimetext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:maxspeed
        anchors.verticalCenter: drivertime.verticalCenter
        anchors.left: sec4.right
        anchors.leftMargin: 10
        text: qsTr("最高速度")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:maxspeedtext
        width: 100
        anchors.verticalCenter: drivertime.verticalCenter
        anchors.left: maxspeed.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec5
        anchors.verticalCenter: maxspeed.verticalCenter
        anchors.left: maxspeedtext.right
        anchors.leftMargin: 5
        text: qsTr("km/h")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:speedsad
        anchors.top: drivertime.bottom
        anchors.topMargin: 10
        anchors.left: drivertime.left
        text: qsTr("超速报警预警阀值")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:speedsadtext
        width: 100
        anchors.verticalCenter: speedsad.verticalCenter
        anchors.left: speedsad.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec6
        anchors.verticalCenter: speedsadtext.verticalCenter
        anchors.left: speedsadtext.right
        anchors.leftMargin: 5
        text: qsTr("1/10km/h")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:driversad
        anchors.top: speedsad.bottom
        anchors.topMargin: 10
        anchors.left: speedsad.left
        text: qsTr("疲劳驾驶预警差值")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:driversadtext
        width: 100
        anchors.verticalCenter: driversad.verticalCenter
        anchors.left: driversad.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec7
        anchors.verticalCenter: driversadtext.verticalCenter
        anchors.left: driversadtext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:collisiontime
        anchors.verticalCenter: driversad.verticalCenter
        anchors.left: sec7.right
        anchors.leftMargin: 10
        text: qsTr("碰撞时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:collisiontimetext
        width: 100
        anchors.verticalCenter: collisiontime.verticalCenter
        anchors.left: collisiontime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec8
        anchors.verticalCenter: collisiontime.verticalCenter
        anchors.left: collisiontimetext.right
        anchors.leftMargin: 5
        text: qsTr("毫秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:collisionacc
        anchors.top: driversad.bottom
        anchors.topMargin: 10
        anchors.left: driversad.left
        text: qsTr("碰撞加速度")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:collisionacctext
        width: 100
        anchors.verticalCenter: collisionacc.verticalCenter
        anchors.left: collisionacc.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec9
        anchors.verticalCenter: collisionacc.verticalCenter
        anchors.left: collisionacctext.right
        anchors.leftMargin: 5
        text: qsTr("0.1g")
        font.pixelSize: fontpixelSize
    }
    Label{
        id:rolloverangle
        anchors.verticalCenter: sec9.verticalCenter
        anchors.left: sec9.right
        anchors.leftMargin: 40
        text: qsTr("侧翻角度")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:rolloverangletext
        width: 100
        anchors.verticalCenter: rolloverangle.verticalCenter
        anchors.left: rolloverangle.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec10
        anchors.verticalCenter: rolloverangle.verticalCenter
        anchors.left: rolloverangletext.right
        anchors.leftMargin: 5
        text: qsTr("度")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:cornerangle
        anchors.top: collisionacc.bottom
        anchors.topMargin: 10
        anchors.left: collisionacc.left
        text: qsTr("拐点补传脚")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:cornerangletext
        width: 100
        anchors.verticalCenter: cornerangle.verticalCenter
        anchors.left: cornerangle.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec11
        anchors.verticalCenter: cornerangle.verticalCenter
        anchors.left: cornerangletext.right
        anchors.leftMargin: 5
        text: qsTr("度")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:drivertotaltime
        anchors.verticalCenter: sec11.verticalCenter
        anchors.left: sec11.right
        anchors.leftMargin: 10
        text: qsTr("当天累计驾驶时间门限")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:drivertotaltimetext
        width: 100
        anchors.verticalCenter: drivertotaltime.verticalCenter
        anchors.left: drivertotaltime.right
        anchors.leftMargin: 5
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:sec12
        anchors.verticalCenter: drivertotaltime.verticalCenter
        anchors.left: drivertotaltimetext.right
        anchors.leftMargin: 5
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
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
