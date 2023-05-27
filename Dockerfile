FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install -y git
RUN apt-get install -y clang
RUN apt-get install -y cmake
RUN apt-get install -y ninja-build
RUN apt-get install -y curl
RUN apt-get install -y lcov
RUN apt-get install -y llvm
