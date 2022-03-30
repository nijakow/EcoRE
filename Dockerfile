FROM debian:latest
RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get install -y git gcc make libz-dev libffi-dev python3-minimal
RUN mkdir /eco-sources
WORKDIR /eco-sources
ADD . /eco-sources
WORKDIR /eco-sources/src/lib
RUN make
WORKDIR /eco-sources/src/ecore
RUN make
CMD ["./ecore.app", "/eco-sources/src/lib/master.ebf"]
