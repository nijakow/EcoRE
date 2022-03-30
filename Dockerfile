FROM debian:latest
RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get install -y gcc make libz-dev libffi-dev python3
RUN mkdir /eco-sources
WORKDIR /eco-sources
ADD . /eco-sources
WORKDIR /eco-sources/src/ecore
RUN make
CMD ["./ecore.app", "../lib/master.ebf"]
