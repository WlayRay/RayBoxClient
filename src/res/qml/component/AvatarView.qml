import QtQuick
import FluentUI

FluClip {

    property var uId
    property bool uisGroup
    radius:[0,0,0,0]

    Image {
        id:image_source
        source: uisGroup? "qrc:/res/image/avatars/"+(uId%105+1)+".webp" : "qrc:/res/image/avatars2/"+(uId%105+60001)+".gif"
        anchors.fill: parent
        // mipmap: true
        cache: true
        visible: uisGroup && image_source.status === Image.Ready
    }

    AnimatedImage{
        id:image_source2
        source: uisGroup? "qrc:/res/image/avatars/"+(uId%105+1)+".webp" : "qrc:/res/image/avatars2/"+(uId%105+60001)+".gif"
        anchors.fill: parent
        // mipmap:true
        cache:true
        visible: !uisGroup && image_source2.status === Image.Ready
    }

}
