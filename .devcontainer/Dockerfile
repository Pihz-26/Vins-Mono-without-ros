FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
ENV HTTP_PROXY="http://127.0.0.1:7897"
ENV HTTPS_PROXY="http://127.0.0.1:7897"

# 更换 Ubuntu 20.04 的源
RUN sed -i 's/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu/g' /etc/apt/sources.list
RUN sed -i 's/http:\/\/security.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn\/ubuntu/g' /etc/apt/sources.list

RUN   apt-get update && apt-get install -y --no-install-recommends\
      lsb-release \
      curl \
      gnupg2 \
      libglew-dev \
      libpython3-dev \
      ffmpeg \
      libavcodec-dev \
      libavutil-dev \
      libavformat-dev \
      libswscale-dev \
      libavdevice-dev \
      libraw1394-dev \
      libopencv-dev \
      libeigen3-dev \
      libgoogle-glog-dev \
      libgflags-dev \
      libatlas-base-dev \
      libeigen3-dev \
      libsuitesparse-dev \
      x11-apps \
      git \
      ca-certificates \
      tzdata \
      cmake \
      g++ \
      libepoxy-dev \
      build-essential \
      libgl1-mesa-dev \
      libgtest-dev \
      libmetis-dev \
      libboost-all-dev \
      doxygen \
      gsfonts \
      graphviz-doc \
      libosmesa6 \
      libglfw3-doc \
      libwayland-doc \
      libxext-doc \
      libxext-dev



# RUN   mkdir /dependencies && \
#       cd /dependencies && \
#       git clone https://github.com/stevenlovegrove/Pangolin.git && \
#       cd Pangolin && \
#       mkdir build && \
#       cd build && \
#       cmake .. && \
#       make -j$(nproc) && \
#       make install

# RUN   cd /dependencies && \
#       git clone https://ceres-solver.googlesource.com/ceres-solver && \
#       cd ceres-solver && \
#       mkdir build && \
#       cd build && \
#       cmake .. && \
#       make -j$(nproc) && \
#       make install




CMD ["tail", "-f", "/dev/null"]


