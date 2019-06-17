import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls 1.2 as Control1

Item {
    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0


    Text {
        id: devicename
        width: boxwidth-100
        text: qsTr("设备名称")
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 100
        font.pixelSize:fontpixelSize
    }
    TextField{
        id:textfielddevicename
        width: boxwidth
        anchors.left: devicename.right
        anchors.leftMargin: 100
        anchors.verticalCenter: devicename.verticalCenter
        height:devicename.height
    }

    Text {
        id: devicenum
        width: boxwidth-100
        text: qsTr("设备编号")
        anchors.top: devicename.bottom
        anchors.topMargin: 20
        anchors.left: devicename.left
        font.pixelSize:fontpixelSize
    }
    TextField{
        id:textfielddevicenum
        width: boxwidth
        anchors.left: devicenum.right
        anchors.leftMargin: 100
        anchors.verticalCenter: devicenum.verticalCenter
        height:devicenum.height
        visible: true
        focus:true

        validator: RegExpValidator{regExp:/\d{1,9}/}
    }

    Text {
        id: light
        width: boxwidth-100
        text: qsTr("显示亮度")
        anchors.top: devicenum.bottom
        anchors.topMargin: 20
        anchors.left: devicenum.left
        font.pixelSize:fontpixelSize
    }

    Control1.Slider{
        id:lightslider
        width: boxwidth
        anchors.verticalCenter: light.verticalCenter
        anchors.left: light.right
        anchors.leftMargin: 100
        minimumValue:0
        maximumValue: 100
        stepSize: 1
        value: 50
    }
    Label{
        id:lightlabel
        anchors.verticalCenter: lightslider.verticalCenter
        anchors.left: lightslider.right
        anchors.leftMargin: 10
        font.pixelSize:fontpixelSize
        text: lightslider.value

    }

    Text {
        id: contrast
        width: boxwidth-100
        text: qsTr("显示对比度")
        anchors.top: light.bottom
        anchors.topMargin: 20
        anchors.left: light.left
        font.pixelSize:fontpixelSize
    }

    Control1.Slider{
        id:contrastslider
        width: boxwidth
        anchors.verticalCenter: contrast.verticalCenter
        anchors.left: contrast.right
        anchors.leftMargin: 100
        minimumValue:0
        maximumValue: 100
        stepSize: 1
        value: 50
    }
    Label{
        id:contrastlabel
        anchors.verticalCenter: contrastslider.verticalCenter
        anchors.left: contrastslider.right
        anchors.leftMargin: 10
        font.pixelSize:fontpixelSize
        text: contrastslider.value

    }

    Text {
        id: saturation
        width: boxwidth-100
        text: qsTr("显示饱和度")
        anchors.top: contrast.bottom
        anchors.topMargin: 20
        anchors.left: contrast.left
        font.pixelSize:fontpixelSize
    }

    Control1.Slider{
        id:saturationslider
        width: boxwidth
        anchors.verticalCenter: saturation.verticalCenter
        anchors.left: saturation.right
        anchors.leftMargin: 100
        minimumValue:0
        maximumValue: 100
        stepSize: 1
        value: 50
    }

    Label{
        id:saturationlabel
        anchors.verticalCenter: saturationslider.verticalCenter
        anchors.left: saturationslider.right
        anchors.leftMargin: 10
        font.pixelSize:fontpixelSize
        text: saturationslider.value

    }





    Connections{
        target: preview
        onPressed:{
            if(setindex == index){
                console.log("preview other")
            }
        }
    }

}
