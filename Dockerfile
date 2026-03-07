FROM ubuntu:22.04

WORKDIR /app

RUN apt-get update && apt-get install -y g++ && rm -rf /var/lib/apt/lists/*

COPY . .

RUN g++ -I src src/main.cpp -o cmp_sorts
RUN g++ -I src -I test test/test.cpp -o my_test

CMD ["./cmp_sorts"]

