#Howto

This docker image allows to build OpenSSL (currently version 1.0.2q) for Android. In order to do so, follow the given instructions: 

* Build `Dockerfile` with: 

`docker build -t android-openssl .`

* Start docker container with: 

`docker run -it android-openssl`

Inside the docker container, run the following script: 

`/tmp/build.sh`

When it's done, you can find the built library in `/tmp/openssl-1.0.2q/build`. 