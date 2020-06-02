FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i -r 's|(archive\|security)\.ubuntu\.com/|ftp.jaist.ac.jp/pub/Linux/|' /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y pigz cmake git libgtk2.0-dev pkg-config \
    libavcodec-dev libavformat-dev libswscale-dev software-properties-common && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app
RUN mkdir -p /etc/udev/rules.d
RUN ./install_zense_sdk.sh
RUN ./build.sh

CMD ["/bin/bash"]