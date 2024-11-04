# Vins Course
[ ![Build Status](doc/README-English-yellow.svg) ](README_EN.md)


**描述**：
本代码在原本代码`https://github.com/HeYijia/VINS-Course`的基础上进行了修改，编写了docker并改进了部分openCVAPI。需要手动安装的第三方库都放在了`thirdparty`中。本身提供了docker进行使用，但是仅仅适用于英伟达显卡驱动，且本身占用了16G内存以保证运行内存足够，可以通过修改`.devcontainer/docker-compose.yaml`文件中的`mem_limit: 16g`来修改到合适的内存占用。
如果本身没有英伟达显卡，可以将`.devcontainer/docker-compose.yaml`中所有和`NVIDIA`有关内容注释。
其本身是适用与linux操作系统的docker编写，因为windows中调用桌面环境过于复杂，所以不进行相应的编写(私密马赛)

### docker
本身提供了docker进行使用，但是仅仅适用于英伟达显卡驱动，且本身占用了16G内存以保证运行内存足够，可以通过修改`.devcontainer/docker-compose.yaml`文件中的`mem_limit: 16g`来修改到合适的内存占用。
如果本身没有英伟达显卡，可以将`.devcontainer/docker-compose.yaml`中所有和`NVIDIA`有关内容注释。
其本身是适用与linux操作系统的docker编写，因为windows中调用桌面环境过于复杂，所以不进行相应的编写(私密马赛)


### 安装依赖项：

1. pangolin: <https://github.com/stevenlovegrove/Pangolin> 下载0.6版本即可，已经存放在`thirdparty`中

2. opencv

3. Eigen

4. Ceres: vins 初始化部分使用了 ceres 做 sfm，所以我们还是需要依赖 ceres. 下载1.14.0版本即可，已经存在`thirdparty`中

### 编译代码

```bash
mkdir build
cd build
cmake ..
make -j4
```

### 运行
#### 1. CurveFitting Example to Verify Our Solver.
```bash
cd bin
./testCurveFitting 
```

#### 2. VINs-Mono on Euroc Dataset
```bash
cd bin
./run_euroc /workspace/dataset/EuRoC/MH-05/mav0/ ../config/
```
![vins](doc/vins.gif)


**以下部分没有进行过修改和测试，存在问题请和我讨论**
#### 3. VINs-Mono on Simulation Dataset 

you can use this code to generate vio data.

```bash
https://github.com/HeYijia/vio_data_simulation
```

#### 4. Validation Results
[evo package](https://github.com/MichaelGrupp/evo)
```bash
evo_ape euroc euroc_mh05_groundtruth.csv pose_output.txt -a -p
```

![results](doc/results.png)


