#include<cstdlib>
#include<stdlib.h>
#include "lzma.h"

lzma::lzma(){
    for(int i=0;i<C_COMPUTE_UNIT;i++) {
        list[i] = NULL;
    }
    size = 0;
}

lzma::~lzma(){
    for(int i=0;i<C_COMPUTE_UNIT;i++) {
        if(list[i])
            delete list[i];
    }
}


lzma *lzma::instance = NULL;

lzma* lzma::getinstance() {
    if(!instance){
        //lock
        instance = new lzma();
    }
    return instance;
}



int lzma::xlzma_init() {
    int handle = atoi(std::getenv("XLZMA_CU"));//-1;
    /*for(int i=0;i<C_COMPUTE_UNIT;i++) {
        if(list[i] == NULL)
            handle = i;
    }

    if(handle == -1)
        return -1;
    */
    if(!list[handle])
        list[handle] = new xil_lzma();
    else
        return -1;

    if(list[handle] == NULL)
        return -1;

    std::string binaryFileName;
    if(SINGLE_XCLBIN) binaryFileName = "xil_lzma_compress_decompress_" + std::to_string(PARALLEL_BLOCK) + "b";
    else binaryFileName = "xil_lzma_compress_" + std::to_string(PARALLEL_BLOCK) + "b";
    list[handle]->m_bin_flow = 1;
    // Create xil_lzma object
    list[handle]->init(binaryFileName);
    list[handle]->m_block_size_in_kb = BLOCK_SIZE_IN_KB;    
    size = 0;
    tag  = START;
    // 0 means Xilinx flow
    list[handle]->m_switch_flow = 0;

    return handle;
}

int lzma::xlzma_set_dict_size(int xlzma_handle, uint64_t dict_size_bytes) {
    return 0;
}

uint64_t lzma::xlzma_bound(uint64_t input_size) {
    return (input_size+(input_size/2));
}

int64_t lzma::xlzma_compress(int xlzma_handle, char* input, uint64_t input_size, char* output, uint64_t output_buf_size) {
    if(output_buf_size  >= xlzma_bound(input_size)){
        uint64_t enbytes = 0;
        enbytes = list[xlzma_handle]->compress_buffer(input,output,input_size,output_buf_size,xlzma_handle/*atoi(std::getenv("XLZMA_CU"))*//*xlzma_handle*/,tag);
        size = size + input_size;
        tag = MID;
        return enbytes;
    }
    else{
        return -2;
    }

   // return output_buf_size >= xlzma_bound(input_size)? list[xlzma_handle]->compress_buffer(input,output,input_size,output_buf_size,atoi(std::getenv("XLZMA_CU"))/*xlzma_handle*/):-2;
}

int64_t lzma::xlzma_finish(int xlzma_handle, char* input, uint64_t input_size, char* output, uint64_t output_buf_size) {
    if(size == 0)
        tag = END | START;
    else
        tag = END;
    return output_buf_size >= xlzma_bound(input_size)? list[xlzma_handle]->compress_buffer(input,output,input_size,output_buf_size,xlzma_handle/*atoi(std::getenv("XLZMA_CU"))*//*xlzma_handle*/,tag):-2;
}

int lzma::xlzma_close(int xlzma_handle) {
    list[xlzma_handle]->release();
}
