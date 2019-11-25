# Howto

This docker image allows to build OpenSSL (currently version 1.0.2t) for Android. In order to do so, follow the given instructions: 

* Build `Dockerfile` with: 

`docker build -t openssl-android-docker-build .`

* Start docker build: 

`docker run --mount type=bind,source="$(pwd)/output",target=/tmp/openssl -it openssl-android-docker-build`

When it's done, you can find the built library in the `output` folder. 
