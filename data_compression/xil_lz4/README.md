# Xilinx LZ4  

Xilinx LZ4 compression/decompression is FPGA based implementation of standard LZ4. 
Xilinx implementation of LZ4 application is aimed at achieving high throughput for both compression and decompression.
This Xilinx LZ4 application is developed and tested on AWS F1 instance. To know
more about standard LZ4 application please refer https://github.com/lz4/lz4

This application is accelerated using generic hardware architecture for LZ based data compression algorithms.

![LZx compress select](../img/lzx_comp.png) <br />

![LZx decompress select](../img/lzx_decomp.png) <br />

For more details refer this [link](https://gitenterprise.xilinx.com/heeran/xil_lz4/blob/master/README.md)


## Results

### Resource Utilization <br />

Table below presents resource utilization of Xilinx LZ4 compress/decompress
kernels with 8 engines for single compute unit. It is possible to extend number of engines to achieve higher throughput.


| Design | LUT | LUTMEM | REG | BRAM | URAM| DSP | Fmax (MHz) |
| --------------- | --- | ------ | --- | ---- | --- | -----| -----|
| Compression     | 52095(5.15%) | 14645(2.56%)|65172(3.07%)|58(3.20%) | 48(5.00%)|1(0.01%)|274|
| Decompression     | 35365(3.43%) | 14018(2.44%)|40937(1.90%)|146(7.93%)|0|1(0.01%)|299|


### Throughput & Compression Ratio

Table below presents the best kernel throughput achieved with single compute unit during execution of this application.

| Topic| Results| 
|-------|--------|
|Best Compression Throughput|1.66 GB/s|
|Best Decompression Throughput| 1.68 GB/s |
|Average Compression Ratio| 2.13x (Silesia Benchmark)|

Note: Overall throughput can still be increased with multiple compute units.

## Software & Hardware

```
  Software: Xilinx SDx 2018.3
  Hardware: xilinx_u200_xdma_201830_1 (Xilinx Alveo U200)
```
 
## Usage


### Build Steps

#### Emulation flows
```
  make check TARGETS=<sw_emu/hw_emu> DEVICES=xilinx_u200_xdma_201830_1
  
  Note: This command compiles for targeted emulation mode and executes the
        application. To execute it on AWS F1 instance, please follow instructions
        specific to AWS F1 emulation.
```
#### Hardware

```
  make all TARGETS=hw DEVICES=xilinx_u200_xdma_201830_1

  Note: This command compiles for hardware execution. It generates kernel binary ".xclbin" file. 
        This file is placed in ./xclbin directory under LZ4 folder. To execute it on AWS F1 instance, 
        please follow instructions specific to AWS F1 deployment process.

```

### Execution Steps

While using PARALLEL_BLOCK (8 default) the generated executable would be
"xil_lz4_8b"

```
  Input Arguments: 
    
        1. To execute single file for compression :  ./xil_lz4_8b -c <file_name>
        2. To execute single file for decompression: ./xil_lz4_8b -d <file_name.lz4>
        3. To validate various files together:       ./xil_lz4_8b -l <files.list>
            3.a. <files.list>: Contains various file names with current path    
        
   Note: Default arguments are set in Makefile

  Help:

        ===============================================================================================
        Usage: application.exe -[-h-c-l-d-B-x]

                --help,         -h      Print Help Options   Default: [false]
                --compress,     -c      Compress
                --file_list,    -l      List of Input Files
                --decompress,   -d      Decompress
                --block_size,   -B      Compress Block Size [0-64: 1-256: 2-1024: 3-4096] Default: [0]
                --flow,     -x      Validation [0-All: 1-XcXd: 2-XcSd: 3-ScXd]   Default:[1]
        ===============================================================================================

```


### Limitations

#### Decompression

No support for block dependence case




