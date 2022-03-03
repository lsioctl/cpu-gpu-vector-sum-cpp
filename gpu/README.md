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

# Docs

https://developer.nvidia.com/blog/even-easier-introduction-cuda/
https://cuda-tutorial.readthedocs.io/en/latest/tutorials/tutorial01/
https://developer.nvidia.com/blog/tag/cuda-refresher/