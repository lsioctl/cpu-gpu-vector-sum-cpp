# Issues

## Installing the right cuda toolkit

I tried first with a too much up to date toolkit: 11.6

And got errors like:

provided-ptx-was-compiled-with-an-unsupported-toolchain

hints: nvidia-smi gives the right supported version
use conda

```
conda install -c nvidia/label/cuda-11.4.4 cuda-toolkit
```

## Getting errors

CUDA has kind of tedious way of managing errors: all
calls (except kernel codes) have a return that has to be checked
(I saw exceptions also vi vscode but can't remember where)

See:

https://stackoverflow.com/questions/14038589/what-is-the-canonical-way-to-check-for-errors-using-the-cuda-runtime-api


## nvprof is not working

So the only way (except getting the errors) to be sure the code is rightly done on GPU
is by launching a long calculation and checking multiple times with nvidia-smi

```
nvidia-smi 
Thu Mar  3 17:48:23 2022       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 470.103.01   Driver Version: 470.103.01   CUDA Version: 11.4     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  Quadro T1000        Off  | 00000000:01:00.0 Off |                  N/A |
| N/A   59C    P0    16W /  N/A |   1979MiB /  3914MiB |    100%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|    0   N/A  N/A      1359      G   /usr/lib/xorg/Xorg                  4MiB |
|    0   N/A  N/A      4446      G   /usr/lib/xorg/Xorg                  4MiB |
|    0   N/A  N/A     31193      C   ./build/cuda-sum-deprecated        47MiB |
+-----------------------------------------------------------------------------+
```

nvprof gives:

```
======== Warning: No profile data collected.
```

Some people seemed to have the same issue starting CUDA 10.1, but
in the documentations I have:
* a supported architecture (Turing)
* a supported CUDA (11.4)

nvvm gives incoherent errors (Java, Eclipse, Canberra Gtk, ...)

# Fixing nvprof not working

Found a working version with anaconda:

```
conda install -c conda-forge cudatoolkit-dev
```

it is slightly lower than the one with nvidia, but packaged by conda-forge

The one from anaconda nvidia channel:

```
nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2021 NVIDIA Corporation
Built on Mon_Oct_11_21:27:02_PDT_2021
Cuda compilation tools, release 11.4, V11.4.152
Build cuda_11.4.r11.4/compiler.30521435_0
```

The one from anaconda conda-forge channel:

```
nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2021 NVIDIA Corporation
Built on Wed_Jun__2_19:15:15_PDT_2021
Cuda compilation tools, release 11.4, V11.4.48
Build cuda_11.4.r11.4/compiler.30033411_0
```

Outputs:

```
nvcc solution-vector_add.cu -o build/solution-vector_add.old
nvprof build/solution-vector_add.old 
==30897== NVPROF is profiling process 30897, command: build/solution-vector_add.old
PASSED
==30897== Profiling application: build/solution-vector_add.old
==30897== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   97.23%  672.83ms         1  672.83ms  672.83ms  672.83ms  vector_add(float*, float*, float*, int)
                    1.73%  11.966ms         1  11.966ms  11.966ms  11.966ms  [CUDA memcpy DtoH]
                    1.04%  7.2205ms         2  3.6102ms  3.4911ms  3.7294ms  [CUDA memcpy HtoD]
      API calls:   86.07%  692.54ms         3  230.85ms  3.5685ms  685.22ms  cudaMemcpy
                   13.86%  111.55ms         3  37.183ms  93.340us  111.36ms  cudaMalloc
                    0.04%  304.85us         3  101.62us  73.011us  153.97us  cudaFree
                    0.01%  104.79us       101  1.0370us      89ns  45.402us  cuDeviceGetAttribute
                    0.01%  92.819us         1  92.819us  92.819us  92.819us  cuDeviceTotalMem
                    0.01%  47.342us         1  47.342us  47.342us  47.342us  cuDeviceGetName
                    0.00%  17.273us         1  17.273us  17.273us  17.273us  cudaLaunchKernel
                    0.00%  5.3470us         1  5.3470us  5.3470us  5.3470us  cuDeviceGetPCIBusId
                    0.00%  1.6450us         3     548ns     140ns  1.3440us  cuDeviceGetCount
                    0.00%     518ns         2     259ns     105ns     413ns  cuDeviceGet
                    0.00%     179ns         1     179ns     179ns     179ns  cuDeviceGetUuid

$ nvcc solution-vector_add_thread.cu -o build/solution-vector_add_thread.old
nvprof build/solution-vector_add_thread.old 
==30966== NVPROF is profiling process 30966, command: build/solution-vector_add_thread.old
PASSED
==30966== Profiling application: build/solution-vector_add_thread.old
==30966== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   48.09%  1.4452ms         1  1.4452ms  1.4452ms  1.4452ms  vector_add(float*, float*, float*, int)
                   27.55%  827.91us         1  827.91us  827.91us  827.91us  [CUDA memcpy DtoH]
                   24.36%  732.00us         2  366.00us  362.95us  369.06us  [CUDA memcpy HtoD]
      API calls:   96.40%  112.71ms         3  37.570ms  70.249us  112.56ms  cudaMalloc
                    3.17%  3.7108ms         3  1.2369ms  390.96us  2.8825ms  cudaMemcpy
                    0.18%  210.41us         3  70.135us  51.499us  101.73us  cudaFree
                    0.09%  106.55us       101  1.0540us      91ns  45.543us  cuDeviceGetAttribute
                    0.07%  78.472us         1  78.472us  78.472us  78.472us  cuDeviceTotalMem
                    0.06%  73.960us         1  73.960us  73.960us  73.960us  cuDeviceGetName
                    0.01%  16.025us         1  16.025us  16.025us  16.025us  cudaLaunchKernel
                    0.00%  5.6770us         1  5.6770us  5.6770us  5.6770us  cuDeviceGetPCIBusId
                    0.00%  1.2760us         3     425ns     107ns  1.0050us  cuDeviceGetCount
                    0.00%     492ns         2     246ns     113ns     379ns  cuDeviceGet
                    0.00%     166ns         1     166ns     166ns     166ns  cuDeviceGetUuid
(cuda3) steff@ls-laptop:~/dev/cpu-gpu-vector-sum-cpp/gpu$ nvprof build/solution-vector_add_thread.old

nvprof build/solution-vector_add_grid 
==33773== NVPROF is profiling process 33773, command: build/solution-vector_add_grid
PASSED
==33773== Profiling application: build/solution-vector_add_grid
==33773== Profiling result:
            Type  Time(%)      Time     Calls       Avg       Min       Max  Name
 GPU activities:   58.38%  11.678ms         1  11.678ms  11.678ms  11.678ms  [CUDA memcpy DtoH]
                   36.33%  7.2667ms         2  3.6333ms  3.5533ms  3.7134ms  [CUDA memcpy HtoD]
                    5.29%  1.0592ms         1  1.0592ms  1.0592ms  1.0592ms  vector_add(float*, float*, float*, int)
      API calls:   82.50%  99.357ms         3  33.119ms  92.509us  99.161ms  cudaMalloc
                   17.03%  20.510ms         3  6.8365ms  3.6277ms  13.147ms  cudaMemcpy
                    0.24%  291.22us         3  97.072us  71.785us  145.70us  cudaFree
                    0.10%  114.97us       101  1.1380us      87ns  50.406us  cuDeviceGetAttribute
                    0.08%  92.493us         1  92.493us  92.493us  92.493us  cuDeviceTotalMem
                    0.04%  47.060us         1  47.060us  47.060us  47.060us  cuDeviceGetName
                    0.01%  16.171us         1  16.171us  16.171us  16.171us  cudaLaunchKernel
                    0.01%  7.3070us         1  7.3070us  7.3070us  7.3070us  cuDeviceGetPCIBusId
                    0.00%  1.5840us         3     528ns     148ns  1.2830us  cuDeviceGetCount
                    0.00%     585ns         2     292ns     119ns     466ns  cuDeviceGet
                    0.00%     154ns         1     154ns     154ns     154ns  cuDeviceGetUuid
```

# Docs

https://developer.nvidia.com/blog/even-easier-introduction-cuda/

https://cuda-tutorial.readthedocs.io/en/latest/tutorials/tutorial01/

https://developer.nvidia.com/blog/tag/cuda-refresher/