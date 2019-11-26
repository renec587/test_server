FROM ubuntu:18.04

MAINTAINER Rene <rene.chaddock@gmail.com>

RUN apt-get update && apt-get -y install make gcc && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /usr/src/myapp
WORKDIR /usr/src/myapp
COPY . .
RUN make && rm *.c *.h *.o client
RUN apt-get -y --purge remove make gcc

CMD [ "./server" ]
