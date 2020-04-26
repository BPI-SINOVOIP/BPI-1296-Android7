        .arch   armv7-a
        .fpu    neon
        .text

        .globl  yuyv422_to_yuv420sp
        .type   yuyv422_to_yuv420sp, STT_FUNC
        .func   yuyv422_to_yuv420sp
yuyv422_to_yuv420sp:
        push            {r4-r5,lr}
        mul             r12, r2,  r3
        add             r4,  r0,  r2,  lsl #1   @ in_1
        add             r5,  r1,  r2            @ out_1
        add             lr,  r1,  r12           @ out_uv
1:
        mov             r12, r2
2:
        vld1.8          {q0},     [r0]!
        vld1.8          {q1},     [r4]!
        vuzp.8          d0,  d1
        vuzp.8          d2,  d3
        vhadd.u8        d1,  d1,  d3
        vrev16.8        d1,  d1
        vst1.8          {d0},     [r1]!
        vst1.8          {d2},     [r5]!
        vst1.8          {d1},     [lr]!
        subs            r12, r12, #8
        bgt             2b
        add             r0,  r0,  r2,  lsl #1
        add             r4,  r4,  r2,  lsl #1
        add             r1,  r1,  r2
        add             r5,  r5,  r2
        subs            r3,  r3,  #2
        bgt             1b
        pop             {r4-r5,pc}
.endfunc
