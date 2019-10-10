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
    Connections{
        target: bubiaoregister
        onPressed:{
            if(setindex == index){
                console.log("设备配置")
                platformset.provienceID = provinceidtext.text
                platformset.cityID = cityidtext.text
                platformset.produceID = produceridtext.text
                 platformset.deviceID = deviceidtext.text
                platformset.deviceModel = devicemodeltext.text
                platformset.carColor = combocarcolor.currentIndex
                platformset.deviceType = combocartype.currentIndex
                platformset.carVinNumber = vinidtext.text
                platformset.tcpAnswerTimeout = tcptimeouttext.text
                platformset.tcpRepeatTimes = tcpreptnumtext.text
                platformset.udpAnswerTimeout = udptimeouttext.text
                platformset.udpRepeatTimes = udpreptnumtext.text
                platformset.smsAnswerTimeout = smstimeouttext.text
                platformset.smsRepeatTimes = smsreptnumtext.text
                platformset.heartBeat = heartbeattext.text


            }
        }
    }

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

    PlatformSet{
        id:platformset
    }

    Label{
        id:provinceid
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 80
        text: qsTr("省域ID")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:provinceidtext
        width: 100
        anchors.verticalCenter: provinceid.verticalCenter
        anchors.left: provinceid.right
        anchors.leftMargin: 5
        text: platformset.provienceID
//        font.pixelSize: fontpixelSize
        onPressed: {
            pressPos(x,y,width,height)
//            keyboardvir.visible = true
        }
    }

    Label{
        id:cityid
        anchors.verticalCenter: provinceid.verticalCenter
        anchors.left: provinceidtext.right
        anchors.leftMargin: 20
        text: qsTr("市县域ID")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:cityidtext
        width: 100
        anchors.verticalCenter: provinceid.verticalCenter
        anchors.left: cityid.right
        anchors.leftMargin: 10
        text: platformset.cityID
//        font.pixelSize: fontpixelSize
        onPressed: pressPos(x,y,width,height)
    }

    Label{
        id:producerid
        anchors.verticalCenter: provinceid.verticalCenter
        anchors.left: cityidtext.right
        anchors.leftMargin: 20
        text: qsTr("制造商ID")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:produceridtext
        width: 100
        anchors.verticalCenter: provinceid.verticalCenter
        anchors.left: producerid.right
        anchors.leftMargin: 10
        text:platformset.produceID
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:deviceid
        anchors.top: provinceid.bottom
        anchors.topMargin: 10
        anchors.left: provinceid.left
        text: qsTr("终端ID")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:deviceidtext
        width: 250
        anchors.verticalCenter: deviceid.verticalCenter
        anchors.left: deviceid.right
        anchors.leftMargin: 10
        text: platformset.deviceID
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:devicemodel
        anchors.verticalCenter: deviceid.verticalCenter
        anchors.left: deviceidtext.right
        anchors.leftMargin: 20
        text: qsTr("终端型号")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:devicemodeltext
        width: 250
        anchors.verticalCenter: deviceid.verticalCenter
        anchors.left: devicemodel.right
        anchors.leftMargin: 10
        text: platformset.deviceModel
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:carcolor
        anchors.top: deviceid.bottom
        anchors.topMargin: 10
        anchors.left: deviceid.left
        text: qsTr("车牌颜色")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:combocarcolor
        width:150
        anchors.verticalCenter: carcolor.verticalCenter
        anchors.left: carcolor.right
        anchors.leftMargin: 5
        currentIndex: platformset.carColor
        model: ["蓝色","黄色","黑色","白色","绿色","其他"]
    }

    Label{
        id:cartype
        anchors.verticalCenter: carcolor.verticalCenter
        anchors.left: combocarcolor.right
        anchors.leftMargin: 20
        text: qsTr("车辆类型")
        font.pixelSize: fontpixelSize
    }

    ComboBox{
        id:combocartype
        width:200
        anchors.verticalCenter: carcolor.verticalCenter
        anchors.left: cartype.right
        anchors.leftMargin: 10
        currentIndex: platformset.deviceType
        model: ["客运车辆","危险品车辆","货运车辆","出租汽车","其他"]
    }


    Label{
        id:vinid
        anchors.top: carcolor.bottom
        anchors.topMargin: 10
        anchors.left: carcolor.left
        text: qsTr("车辆VIN号")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:vinidtext
        width: 250
        anchors.verticalCenter: vinid.verticalCenter
        anchors.left: vinid.right
        anchors.leftMargin: 10
        text: platformset.carVinNumber
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:tcptimeout
        anchors.verticalCenter: vinid.verticalCenter
        anchors.left: vinidtext.right
        anchors.leftMargin: 20
        text: qsTr("TCP超时时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:tcptimeouttext
        width: 80
        anchors.verticalCenter: vinid.verticalCenter
        anchors.left: tcptimeout.right
        anchors.leftMargin: 10
        text: platformset.tcpAnswerTimeout
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:tcpsec
        anchors.verticalCenter: vinid.verticalCenter
        anchors.left: tcptimeouttext.right
//        anchors.leftMargin: 10
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:tcpreptnum
        anchors.top: vinid.bottom
        anchors.topMargin: 10
        anchors.left: vinid.left
        text: qsTr("TCP重传次数")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:tcpreptnumtext
        width: 80
        anchors.verticalCenter: tcpreptnum.verticalCenter
        anchors.left: tcpreptnum.right
        anchors.leftMargin: 10
        text: platformset.tcpRepeatTimes
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }


    Label{
        id:udptimeout
        anchors.top: tcptimeout.bottom
        anchors.topMargin: 10
        anchors.left: tcptimeout.left
        text: qsTr("UDP超时时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:udptimeouttext
        width: 80
        anchors.verticalCenter: udptimeout.verticalCenter
        anchors.left: udptimeout.right
        anchors.leftMargin: 10
        text: platformset.udpAnswerTimeout
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:udpsec
        anchors.verticalCenter: udptimeout.verticalCenter
        anchors.left: udptimeouttext.right
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:udpreptnum
        anchors.top: tcpreptnum.bottom
        anchors.topMargin: 10
        anchors.left: tcpreptnum.left
        text: qsTr("UDP重传次数")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:udpreptnumtext
        width: 80
        anchors.verticalCenter: udpreptnum.verticalCenter
        anchors.left: udpreptnum.right
        anchors.leftMargin: 10
        text: platformset.udpRepeatTimes
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:smstimeout
        anchors.top: udptimeout.bottom
        anchors.topMargin: 10
        anchors.left: udptimeout.left
        text: qsTr("SMS超时时间")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:smstimeouttext
        width: 80
        anchors.verticalCenter: smstimeout.verticalCenter
        anchors.left: smstimeout.right
        anchors.leftMargin: 10
        text: platformset.smsAnswerTimeout
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:smssec
        anchors.verticalCenter: smstimeout.verticalCenter
        anchors.left: smstimeouttext.right
        text: qsTr("秒")
        font.pixelSize: fontpixelSize
    }

    Label{
        id:smsreptnum
        anchors.top: udpreptnum.bottom
        anchors.topMargin: 10
        anchors.left: udpreptnum.left
        text: qsTr("UDP重传次数")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:smsreptnumtext
        width: 80
        anchors.verticalCenter: smsreptnum.verticalCenter
        anchors.left: smsreptnum.right
        anchors.leftMargin: 10
        text: platformset.smsRepeatTimes
        onPressed: pressPos(x,y,width,height)
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:heartbeat
        anchors.top: smstimeout.bottom
        anchors.topMargin: 10
        anchors.left: smstimeout.left
        text: qsTr("心跳发送间隔")
        font.pixelSize: fontpixelSize
    }

    TextField{
        id:heartbeattext
        width: 80
        anchors.verticalCenter: heartbeat.verticalCenter
        anchors.left: heartbeat.right
        anchors.leftMargin: 10
        text: platformset.heartBeat
        onPressed:
        {

            pressPos(x,y,width,height)
        }
//        font.pixelSize: fontpixelSize
    }

    Label{
        id:heartbeatsec
        anchors.verticalCenter: heartbeat.verticalCenter
        anchors.left: heartbeattext.right
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

