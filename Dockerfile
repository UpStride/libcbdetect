FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
RUN apt update
RUN apt install -y libopencv-dev cmake g++ python3-pip
RUN pip3 install numpy

ENTRYPOINT /bin/bash
