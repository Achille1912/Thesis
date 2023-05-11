FROM ubuntu:latest

RUN apt-get update \
	&& apt-get install -y \
		build-essential \
		libsdl2-dev \
		libsdl2-image-dev \

COPY . /app
WORKDIR /app

RUN make

CMD ["./2Dynamics"]