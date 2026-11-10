# CLI practice only; the official NWERC workstation is a modified ICPC image.
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive LANG=C.UTF-8 LC_ALL=C.UTF-8
RUN test "$(dpkg --print-architecture)" = amd64 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       bash ca-certificates curl bzip2 xz-utils \
       gcc-13 g++-13 make gdb time diffutils coreutils unzip zip \
       python3 vim nano \
       libexpat1 libncursesw6 libsqlite3-dev libssl-dev liblzma-dev libffi-dev \
    && rm -rf /var/lib/apt/lists/* \
    && ln -s /usr/bin/g++-13 /usr/local/bin/g++ \
    && ln -s /usr/bin/gcc-13 /usr/local/bin/gcc \
    && ln -s /usr/bin/gcc-13 /usr/local/bin/cc

# Official PyPy 7.3.15, Python 3.9.18, x86-64 release and published SHA256.
RUN curl -fsSL --retry 3 \
      https://downloads.python.org/pypy/pypy3.9-v7.3.15-linux64.tar.bz2 \
      -o /tmp/pypy.tar.bz2 \
    && echo 'f062be307200bde434817e1620cebc13f563d6ab25309442c5f4d0f0d68f0912  /tmp/pypy.tar.bz2' | sha256sum -c - \
    && mkdir /opt/pypy \
    && tar -xjf /tmp/pypy.tar.bz2 -C /opt/pypy --strip-components=1 \
    && rm /tmp/pypy.tar.bz2 \
    && ln -s /opt/pypy/bin/pypy3 /usr/local/bin/pypy3 \
    && ln -s /opt/pypy/bin/pypy3 /usr/local/bin/python3 \
    && cd /opt/pypy/lib/pypy3.9 \
    && pypy3 _ssl_build.py \
    && pypy3 _lzma_build.py \
    && pypy3 _sqlite3_build.py

COPY check-versions.sh /usr/local/bin/check-versions
RUN chmod +x /usr/local/bin/check-versions \
    && check-versions \
    && pypy3 -c 'import collections, fractions, heapq, itertools, math, ssl, lzma, sqlite3' \
    && printf '#include <bits/stdc++.h>\nint main(){std::cout << 42;}' \
       | g++ -x c++ -std=gnu++20 -Wall -O2 -static -pipe -DONLINE_JUDGE -DDOMJUDGE -o /tmp/cpp-check - \
    && test "$(/tmp/cpp-check)" = 42 \
    && rm /tmp/cpp-check

ARG PRACTICE_UID=1000
ARG PRACTICE_GID=1000
# Ubuntu's base image may already have an unused uid-1000 account.
RUN test "$PRACTICE_UID" -ge 1000 && test "$PRACTICE_GID" -ge 1000 \
    && if getent passwd "$PRACTICE_UID" >/dev/null; then \
         userdel "$(getent passwd "$PRACTICE_UID" | cut -d: -f1)"; \
       fi \
    && if ! getent group "$PRACTICE_GID" >/dev/null; then \
         groupadd --gid "$PRACTICE_GID" contestant; \
       fi \
    && useradd --uid "$PRACTICE_UID" --gid "$PRACTICE_GID" \
         --create-home --shell /bin/bash contestant
USER contestant
ENV HOME=/home/contestant
WORKDIR /workspace
CMD ["bash", "--noprofile", "--norc"]
