FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
  git \
  clang-15 \
  cmake \
  ninja-build \
  curl \
  && rm -rf /var/lib/apt/lists/*
