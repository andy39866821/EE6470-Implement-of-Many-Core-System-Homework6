# EE6470 Homework 6 Report
###### tags: `EE6470`
## Problems and Solutions
### Platform installation
Q: How to build accelerator platform?
A: By the following steps.
1. add "basic-hw6" to riscv-vp\vp\src\platform
2. modify Makefile and source code in basic-hw6
3. Add "subdirs(basic-acc)" in CmakeList.txt
4. run "cmake .." and "make install" in vp-build

## Implementation details 

### Golden model
I generate input_bitmap.h and output_bitmap.h for testing in pure C++ program.
* Golden directory
![](https://i.imgur.com/XbSYitR.png)
* input_bitmap.h
![](https://i.imgur.com/3AilQJg.png)
* output_bitmap.h
![](https://i.imgur.com/Lm1R4H2.png)
### Testbench in risc-v software
I feed data pixel by pixel as below code.
![](https://i.imgur.com/LNkv7Ld.png)
Then verify the answer by the golden.
![](https://i.imgur.com/6gLmXoz.png)
Also in Testbench, it allow two data transferring mode: DMA and memory copy.
![](https://i.imgur.com/lSesBpE.png)

### Design in risc-v platform
* main.cpp
I just modify "sobel_filter" to my design, also I reduce one socket target port since I don't have filter_r version.
![](https://i.imgur.com/qAUzu9G.png)
* Filter.h
Just like previous homeworks, I set input and output fifo for R, G, and B.
![](https://i.imgur.com/UVb3dUD.png)
Then design the convolution while loop to MAC 9 pixels with filtering factor.
![](https://i.imgur.com/foRgDfJ.png)
And sending part of blocking transport function is modified due to output port has been added. 
![](https://i.imgur.com/ftL7ykc.png)


## Additional features
### Different directory structure
For simplier testing, I set different directory structure, which is embedded prebuild risc-v platform platform.
```
hw6
   ├── platform 
   │   ├── RISC-V software base files
   │   └── Filter.h                 
   ├── software                    
   │   ├── Makefile                    # Makefile will include riscv-vp-hw6 in bin to run software.
   │   ├── main.cpp    
   │   ├── input_bitmap.h     
   │   └── output_bitmap.h         
   ├── bin                  
   │   └── riscv-vp-hw6                # platform prebuild
   └── golden
       ├── golden.cpp    
       ├── input.bmp    
       └── output.bmp
   
```


## Experimental results
### Simulating times
* DMA
![](https://i.imgur.com/QQE8Rg2.jpg)


* memcpy
![](https://i.imgur.com/blLCYUv.jpg)

Both method can work successfully, but DMA method will take more than 10 times time to calculate.


## Discussions and conclusions
Risc-v provided a well designed integrated platform to test our design on complete system with memory, IO, and CPU. Also DMA and memory copy can test design on different IO interface, which enhanced debugging effiency.
