
# Build AppImage

* build docker image with `docker build -t build-appimage .`
* run with `docker run --privileged --mount type=bind,source="$(pwd)",target=/tmp -it build-appimage`
* the AppImage will be available in the `nextload-client/build` folder (which will be created in the current folder)
