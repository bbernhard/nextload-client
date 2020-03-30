FROM ubuntu:xenial

RUN apt-get update \
	&& apt-get -y install software-properties-common \
	&& add-apt-repository ppa:beineri/opt-qt-5.12.6-xenial \
	&& apt-get update \
	&& apt-get -y install qt512-meta-full libgl1-mesa-dev wget git build-essential fuse \
	&& /bin/bash -c "/opt/qt*/bin/qt*-env.sh"

RUN wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage" \
	&& chmod a+x linuxdeployqt-6-x86_64.AppImage 

ENV PATH=/opt/qt512/bin/:$PATH

ADD build_appimage.sh /
ADD nextload-client.desktop /

RUN chmod +x build_appimage.sh

WORKDIR /

ENTRYPOINT ["./build_appimage.sh"]
