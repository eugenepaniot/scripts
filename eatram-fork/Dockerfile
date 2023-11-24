FROM ubuntu:latest as builder

RUN apt-get update && apt-get install -y gcc make

ADD . /src

WORKDIR /src
RUN     make clean all

#######################

FROM ubuntu:latest

COPY --from=builder /src/eat /usr/local/bin/

CMD ["sleep", "+inf"]
