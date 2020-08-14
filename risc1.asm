auipc   t0,0x2
jr tp
li   a1,0x30
li   a2,0x4000
li   a3,0x600000
li   a4,0x80000000

csrrw s2, 64, a1
csrrs s3, 64, a2
csrrs s4, 64, a3
csrrs s5, 64, a4
csrrc s6, 64, a2

csrrsi s7, 64, 31
