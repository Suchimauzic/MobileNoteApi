FROM gcc:14.2.0 AS builder

WORKDIR /home/serega/MobileNoteApi

RUN set -ex; \
    apt-get update; \
    apt-get install -y cmake;

COPY . .

RUN set -ex;  \
    cd .; \
    rm -rf tests; \
    rm -rf build; \
    mkdir build;    \
    cd build/;  \
    cmake -DMOBILE_UNIT_TEST=FALSE ..;   \
    make;

#=============================

FROM gcc:14.2.0

RUN apt-get update; \
    apt-get install -y libpq5;  \
    rm -rf /var/lib/apt/lists/*

COPY --from=builder /home/serega/MobileNoteApi/build/MobileNoteApi /usr/bin

EXPOSE 9080

CMD ["MobileNoteApi"]