#FROM alpine:3.6
FROM debian:stretch

ENV OPENSSL_VERSION=1.0.2q
ENV NDK_VERSION=r12b
ENV OPENSSL_FULL_VERSION=openssl-${OPENSSL_VERSION}

RUN apt-get update && apt-get install -y wget make ca-certificates perl unzip build-essential dos2unix curl python && rm -rf /var/cache/apk/*

RUN cd /tmp/ && wget https://dl.google.com/android/repository/android-ndk-${NDK_VERSION}-linux-x86_64.zip && unzip android-ndk-${NDK_VERSION}-linux-x86_64.zip

ENV ANDROID_NDK=/tmp/android-ndk-${NDK_VERSION}/
ENV NDK_ROOT=/tmp/android-ndk-${NDK_VERSION}/
ENV ANDROID_NDK_ROOT=/tmp/android-ndk-${NDK_VERSION}/

RUN cd /tmp/ && wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz && tar xvf openssl-${OPENSSL_VERSION}.tar.gz && chmod -R u+rwx openssl-${OPENSSL_VERSION}
COPY build.sh /tmp/build.sh
RUN dos2unix /tmp/build.sh
RUN chmod u+rx /tmp/build.sh
