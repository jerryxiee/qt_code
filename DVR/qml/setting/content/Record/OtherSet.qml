import QtQuick 2.0

Item {

    property int fontpixelSize : 32
    property int boxwidth: 300
    property int index: 0
    Connections{
        target: record
        onPressed:{
            if(setindex == index){

            }
        }
    }
}
