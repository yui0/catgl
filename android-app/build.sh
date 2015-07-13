#!/bin/sh
# sh build.sh (berry.)kikyu Kikyu(.java) kikyu.cpp assets
package=$1
name=$2
source=$3
assets=$4
path=$5
[ -z "${path}" ] && path=/root/Dropbox/prog/catgl

rm -rf /tmp/android-app
cp -a ${path}/android-app /tmp/

[ -r AndroidManifest.xml ] && cp -a AndroidManifest.xml /tmp/android-app/
[ -r icon.png ] && cp -a icon.png /tmp/android-app/res/drawable/

sed -e "s/@@NAME@@/${name}/" -i /tmp/android-app/build.xml
sed -e "s:~/android-ndk:$HOME/android-ndk:" -i /tmp/android-app/build.xml
sed -e "s/@@NAME@@/${name}/" -i /tmp/android-app/AndroidManifest.xml
sed -e "s/@@PACKAGE@@/${package}/" -i /tmp/android-app/AndroidManifest.xml
sed -e "s/native-activity/CatNative/" -i /tmp/android-app/AndroidManifest.xml
sed -e "s/@@NAME@@/${name}/" -i /tmp/android-app/res/values/strings.xml

#mv /tmp/android-app/src/berry/package /tmp/android-app/src/berry/${package}
#mv /tmp/android-app/src/berry/${package}/Act.java /tmp/android-app/src/berry/${package}/${name}.java
#sed -e "s/@@NAME@@/${name}/" -i /tmp/android-app/src/berry/${package}/${name}.java
#sed -e "s/@@PACKAGE@@/${package}/" -i /tmp/android-app/src/berry/${package}/${name}.java

[ -r ${assets} ] && cp -a ${assets} /tmp/android-app/assets
cp -a ${path}/*.h ${path}/nanovg/* ${path}/fonts *.[ch]* /tmp/android-app/jni/
sed -e "s:@@SOURCE@@:${source}:" -i /tmp/android-app/jni/Android.mk
sed -e "s:@@INCLUDES@@:${path}/include:" -i /tmp/android-app/jni/Android.mk
sed -e "s:@@LIBS@@:${path}/project/android/lib -L${path}/project/android/obj/local/armeabi:" -i /tmp/android-app/jni/Android.mk


pushd /tmp/android-app/
#date=`date '+%Y.%m.%d'`
#sed -e "s/%%DATE%%/$date/" assets/help.html.orig > assets/help.html

export PATH=$PATH:~/android-sdk-linux/platform-tools/

ant debug
#ant release
adb install -r bin/${name}-debug.apk
popd

mv /tmp/android-app/bin/${name}-*.apk .
