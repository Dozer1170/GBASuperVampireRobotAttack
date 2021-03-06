	.file	"sinlut.c"
@ GNU C version 3.3.2 (arm-thumb-elf)
@	compiled by GNU C version 3.3.1 (cygming special).
@ GGC heuristics: --param ggc-min-expand=30 --param ggc-min-heapsize=4096
@ options passed:  -fpreprocessed -mthumb-interwork -mlong-calls
@ -auxbase-strip -O2 -Wall -fverbose-asm
@ options enabled:  -fdefer-pop -fomit-frame-pointer
@ -foptimize-sibling-calls -fcse-follow-jumps -fcse-skip-blocks
@ -fexpensive-optimizations -fthread-jumps -fstrength-reduce -fpeephole
@ -fforce-mem -ffunction-cse -fkeep-static-consts -fcaller-saves
@ -freg-struct-return -fgcse -fgcse-lm -fgcse-sm -floop-optimize
@ -fcrossjumping -fif-conversion -fif-conversion2 -frerun-cse-after-loop
@ -frerun-loop-opt -fdelete-null-pointer-checks -fschedule-insns
@ -fschedule-insns2 -fsched-interblock -fsched-spec -fbranch-count-reg
@ -freorder-blocks -freorder-functions -fcprop-registers -fcommon
@ -fverbose-asm -fgnu-linker -fregmove -foptimize-register-move
@ -fargument-alias -fstrict-aliasing -fmerge-constants
@ -fzero-initialized-in-bss -fident -fpeephole2 -fguess-branch-probability
@ -fmath-errno -ftrapping-math -mapcs -mapcs-frame -mapcs-32 -msoft-float
@ -mthumb-interwork -mlong-calls

	.global	sin_lut
	.section	.rodata
	.align	1
	.type	sin_lut, %object
	.size	sin_lut, 1024
sin_lut:
	.short	0
	.short	50
	.short	100
	.short	150
	.short	200
	.short	251
	.short	301
	.short	351
	.short	401
	.short	451
	.short	501
	.short	551
	.short	601
	.short	650
	.short	700
	.short	749
	.short	799
	.short	848
	.short	897
	.short	946
	.short	995
	.short	1043
	.short	1092
	.short	1140
	.short	1189
	.short	1237
	.short	1284
	.short	1332
	.short	1379
	.short	1427
	.short	1474
	.short	1520
	.short	1567
	.short	1613
	.short	1659
	.short	1705
	.short	1751
	.short	1796
	.short	1841
	.short	1886
	.short	1930
	.short	1975
	.short	2018
	.short	2062
	.short	2105
	.short	2148
	.short	2191
	.short	2233
	.short	2275
	.short	2317
	.short	2358
	.short	2399
	.short	2439
	.short	2480
	.short	2519
	.short	2559
	.short	2598
	.short	2637
	.short	2675
	.short	2713
	.short	2750
	.short	2787
	.short	2824
	.short	2860
	.short	2896
	.short	2931
	.short	2966
	.short	3000
	.short	3034
	.short	3068
	.short	3101
	.short	3134
	.short	3166
	.short	3197
	.short	3229
	.short	3259
	.short	3289
	.short	3319
	.short	3348
	.short	3377
	.short	3405
	.short	3433
	.short	3460
	.short	3487
	.short	3513
	.short	3538
	.short	3563
	.short	3588
	.short	3612
	.short	3635
	.short	3658
	.short	3680
	.short	3702
	.short	3723
	.short	3744
	.short	3764
	.short	3784
	.short	3803
	.short	3821
	.short	3839
	.short	3856
	.short	3873
	.short	3889
	.short	3904
	.short	3919
	.short	3933
	.short	3947
	.short	3960
	.short	3973
	.short	3985
	.short	3996
	.short	4007
	.short	4017
	.short	4026
	.short	4035
	.short	4043
	.short	4051
	.short	4058
	.short	4065
	.short	4071
	.short	4076
	.short	4080
	.short	4084
	.short	4088
	.short	4091
	.short	4093
	.short	4094
	.short	4095
	.short	4096
	.short	4095
	.short	4094
	.short	4093
	.short	4091
	.short	4088
	.short	4084
	.short	4080
	.short	4076
	.short	4071
	.short	4065
	.short	4058
	.short	4051
	.short	4043
	.short	4035
	.short	4026
	.short	4017
	.short	4007
	.short	3996
	.short	3985
	.short	3973
	.short	3960
	.short	3947
	.short	3933
	.short	3919
	.short	3904
	.short	3889
	.short	3873
	.short	3856
	.short	3839
	.short	3821
	.short	3803
	.short	3784
	.short	3764
	.short	3744
	.short	3723
	.short	3702
	.short	3680
	.short	3658
	.short	3635
	.short	3612
	.short	3588
	.short	3563
	.short	3538
	.short	3513
	.short	3487
	.short	3460
	.short	3433
	.short	3405
	.short	3377
	.short	3348
	.short	3319
	.short	3289
	.short	3259
	.short	3229
	.short	3197
	.short	3166
	.short	3134
	.short	3101
	.short	3068
	.short	3034
	.short	3000
	.short	2966
	.short	2931
	.short	2896
	.short	2860
	.short	2824
	.short	2787
	.short	2750
	.short	2713
	.short	2675
	.short	2637
	.short	2598
	.short	2559
	.short	2519
	.short	2480
	.short	2439
	.short	2399
	.short	2358
	.short	2317
	.short	2275
	.short	2233
	.short	2191
	.short	2148
	.short	2105
	.short	2062
	.short	2018
	.short	1975
	.short	1930
	.short	1886
	.short	1841
	.short	1796
	.short	1751
	.short	1705
	.short	1659
	.short	1613
	.short	1567
	.short	1520
	.short	1474
	.short	1427
	.short	1379
	.short	1332
	.short	1284
	.short	1237
	.short	1189
	.short	1140
	.short	1092
	.short	1043
	.short	995
	.short	946
	.short	897
	.short	848
	.short	799
	.short	749
	.short	700
	.short	650
	.short	601
	.short	551
	.short	501
	.short	451
	.short	401
	.short	351
	.short	301
	.short	251
	.short	200
	.short	150
	.short	100
	.short	50
	.short	0
	.short	-50
	.short	-100
	.short	-150
	.short	-200
	.short	-251
	.short	-301
	.short	-351
	.short	-401
	.short	-451
	.short	-501
	.short	-551
	.short	-601
	.short	-650
	.short	-700
	.short	-749
	.short	-799
	.short	-848
	.short	-897
	.short	-946
	.short	-995
	.short	-1043
	.short	-1092
	.short	-1140
	.short	-1189
	.short	-1237
	.short	-1284
	.short	-1332
	.short	-1379
	.short	-1427
	.short	-1474
	.short	-1520
	.short	-1567
	.short	-1613
	.short	-1659
	.short	-1705
	.short	-1751
	.short	-1796
	.short	-1841
	.short	-1886
	.short	-1930
	.short	-1975
	.short	-2018
	.short	-2062
	.short	-2105
	.short	-2148
	.short	-2191
	.short	-2233
	.short	-2275
	.short	-2317
	.short	-2358
	.short	-2399
	.short	-2439
	.short	-2480
	.short	-2519
	.short	-2559
	.short	-2598
	.short	-2637
	.short	-2675
	.short	-2713
	.short	-2750
	.short	-2787
	.short	-2824
	.short	-2860
	.short	-2896
	.short	-2931
	.short	-2966
	.short	-3000
	.short	-3034
	.short	-3068
	.short	-3101
	.short	-3134
	.short	-3166
	.short	-3197
	.short	-3229
	.short	-3259
	.short	-3289
	.short	-3319
	.short	-3348
	.short	-3377
	.short	-3405
	.short	-3433
	.short	-3460
	.short	-3487
	.short	-3513
	.short	-3538
	.short	-3563
	.short	-3588
	.short	-3612
	.short	-3635
	.short	-3658
	.short	-3680
	.short	-3702
	.short	-3723
	.short	-3744
	.short	-3764
	.short	-3784
	.short	-3803
	.short	-3821
	.short	-3839
	.short	-3856
	.short	-3873
	.short	-3889
	.short	-3904
	.short	-3919
	.short	-3933
	.short	-3947
	.short	-3960
	.short	-3973
	.short	-3985
	.short	-3996
	.short	-4007
	.short	-4017
	.short	-4026
	.short	-4035
	.short	-4043
	.short	-4051
	.short	-4058
	.short	-4065
	.short	-4071
	.short	-4076
	.short	-4080
	.short	-4084
	.short	-4088
	.short	-4091
	.short	-4093
	.short	-4094
	.short	-4095
	.short	-4096
	.short	-4095
	.short	-4094
	.short	-4093
	.short	-4091
	.short	-4088
	.short	-4084
	.short	-4080
	.short	-4076
	.short	-4071
	.short	-4065
	.short	-4058
	.short	-4051
	.short	-4043
	.short	-4035
	.short	-4026
	.short	-4017
	.short	-4007
	.short	-3996
	.short	-3985
	.short	-3973
	.short	-3960
	.short	-3947
	.short	-3933
	.short	-3919
	.short	-3904
	.short	-3889
	.short	-3873
	.short	-3856
	.short	-3839
	.short	-3821
	.short	-3803
	.short	-3784
	.short	-3764
	.short	-3744
	.short	-3723
	.short	-3702
	.short	-3680
	.short	-3658
	.short	-3635
	.short	-3612
	.short	-3588
	.short	-3563
	.short	-3538
	.short	-3513
	.short	-3487
	.short	-3460
	.short	-3433
	.short	-3405
	.short	-3377
	.short	-3348
	.short	-3319
	.short	-3289
	.short	-3259
	.short	-3229
	.short	-3197
	.short	-3166
	.short	-3134
	.short	-3101
	.short	-3068
	.short	-3034
	.short	-3000
	.short	-2966
	.short	-2931
	.short	-2896
	.short	-2860
	.short	-2824
	.short	-2787
	.short	-2750
	.short	-2713
	.short	-2675
	.short	-2637
	.short	-2598
	.short	-2559
	.short	-2519
	.short	-2480
	.short	-2439
	.short	-2399
	.short	-2358
	.short	-2317
	.short	-2275
	.short	-2233
	.short	-2191
	.short	-2148
	.short	-2105
	.short	-2062
	.short	-2018
	.short	-1975
	.short	-1930
	.short	-1886
	.short	-1841
	.short	-1796
	.short	-1751
	.short	-1705
	.short	-1659
	.short	-1613
	.short	-1567
	.short	-1520
	.short	-1474
	.short	-1427
	.short	-1379
	.short	-1332
	.short	-1284
	.short	-1237
	.short	-1189
	.short	-1140
	.short	-1092
	.short	-1043
	.short	-995
	.short	-946
	.short	-897
	.short	-848
	.short	-799
	.short	-749
	.short	-700
	.short	-650
	.short	-601
	.short	-551
	.short	-501
	.short	-451
	.short	-401
	.short	-351
	.short	-301
	.short	-251
	.short	-200
	.short	-150
	.short	-100
	.short	-50
	.ident	"GCC: (GNU) 3.3.2"
