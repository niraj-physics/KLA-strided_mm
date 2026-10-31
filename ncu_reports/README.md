
# ncu reports file
1. v0_0 : kernel version v0 , initial report -- Atomic Update
2. v1_0 : kernel version v1 , initial report -- Loops 
3. v2_0 : kernel version v2 , initial report -- Atomic Update + Loops + padding (for bank conflict)
4. v3_int16 : kernel version v3 (input : int16_t output : double)  
5. v3_fp16_01 : kernel version  v3 (input : fp16 , output: float)
5. v3_fp16_02 : kernel version  v3 (input : fp16 , output: float) -- Memmory Coalescing fix at atomic add
6. v3_fp16_03 : kernel version v3 -- Register tiling 
7. v3_fp16_03_bank_conflict : +2 to +4 padding at shared memmory 
