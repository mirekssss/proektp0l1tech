FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake ninja-build g++ \
    qt6-base-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY backend_server/ ./backend_server/

RUN cmake -B backend_server/build -S backend_server -G Ninja \
    && cmake --build backend_server/build

EXPOSE 33333

CMD ["./backend_server/build/main_server"]
