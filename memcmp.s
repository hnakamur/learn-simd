Dump of assembler code for function __memcmp_avx2_movbe:
=> 0x0000000000413c00 <+0>:     endbr64
   0x0000000000413c04 <+4>:     cmp    $0x20,%rdx
   0x0000000000413c08 <+8>:     jb     0x413ee0 <__memcmp_avx2_movbe+736>
   0x0000000000413c0e <+14>:    vmovdqu (%rsi),%ymm1
   0x0000000000413c12 <+18>:    vpcmpeqb (%rdi),%ymm1,%ymm1
   0x0000000000413c16 <+22>:    vpmovmskb %ymm1,%eax
   0x0000000000413c1a <+26>:    inc    %eax
   0x0000000000413c1c <+28>:    jne    0x413ce0 <__memcmp_avx2_movbe+224>
   0x0000000000413c22 <+34>:    cmp    $0x40,%rdx
   0x0000000000413c26 <+38>:    jbe    0x413e64 <__memcmp_avx2_movbe+612>
   0x0000000000413c2c <+44>:    vmovdqu 0x20(%rsi),%ymm2
   0x0000000000413c31 <+49>:    vpcmpeqb 0x20(%rdi),%ymm2,%ymm2
   0x0000000000413c36 <+54>:    vpmovmskb %ymm2,%eax
   0x0000000000413c3a <+58>:    inc    %eax
   0x0000000000413c3c <+60>:    jne    0x413d00 <__memcmp_avx2_movbe+256>
   0x0000000000413c42 <+66>:    cmp    $0x80,%rdx
   0x0000000000413c49 <+73>:    jbe    0x413e50 <__memcmp_avx2_movbe+592>
   0x0000000000413c4f <+79>:    vmovdqu 0x40(%rsi),%ymm3
   0x0000000000413c54 <+84>:    vpcmpeqb 0x40(%rdi),%ymm3,%ymm3
   0x0000000000413c59 <+89>:    vpmovmskb %ymm3,%eax
   0x0000000000413c5d <+93>:    inc    %eax
   0x0000000000413c5f <+95>:    jne    0x413d20 <__memcmp_avx2_movbe+288>
   0x0000000000413c65 <+101>:   vmovdqu 0x60(%rsi),%ymm4
   0x0000000000413c6a <+106>:   vpcmpeqb 0x60(%rdi),%ymm4,%ymm4
   0x0000000000413c6f <+111>:   vpmovmskb %ymm4,%ecx
   0x0000000000413c73 <+115>:   inc    %ecx
   0x0000000000413c75 <+117>:   jne    0x413d5b <__memcmp_avx2_movbe+347>
   0x0000000000413c7b <+123>:   cmp    $0x100,%rdx
   0x0000000000413c82 <+130>:   ja     0x413d70 <__memcmp_avx2_movbe+368>
   0x0000000000413c88 <+136>:   vmovdqu -0x80(%rsi,%rdx,1),%ymm1
   0x0000000000413c8e <+142>:   vmovdqu -0x60(%rsi,%rdx,1),%ymm2
   0x0000000000413c94 <+148>:   lea    -0x80(%rdi,%rdx,1),%rdi
   0x0000000000413c99 <+153>:   lea    -0x80(%rsi,%rdx,1),%rsi
   0x0000000000413c9e <+158>:   vpcmpeqb (%rdi),%ymm1,%ymm1
   0x0000000000413ca2 <+162>:   vpcmpeqb 0x20(%rdi),%ymm2,%ymm2
   0x0000000000413ca7 <+167>:   vmovdqu 0x40(%rsi),%ymm3
   0x0000000000413cac <+172>:   vpcmpeqb 0x40(%rdi),%ymm3,%ymm3
   0x0000000000413cb1 <+177>:   vmovdqu 0x60(%rsi),%ymm4
   0x0000000000413cb6 <+182>:   vpcmpeqb 0x60(%rdi),%ymm4,%ymm4
   0x0000000000413cbb <+187>:   vpand  %ymm1,%ymm2,%ymm5
   0x0000000000413cbf <+191>:   vpand  %ymm3,%ymm4,%ymm6
   0x0000000000413cc3 <+195>:   vpand  %ymm5,%ymm6,%ymm7
   0x0000000000413cc7 <+199>:   vpmovmskb %ymm7,%ecx
   0x0000000000413ccb <+203>:   inc    %ecx
   0x0000000000413ccd <+205>:   jne    0x413d43 <__memcmp_avx2_movbe+323>
   0x0000000000413ccf <+207>:   vzeroupper
   0x0000000000413cd2 <+210>:   ret
   0x0000000000413cd3 <+211>:   data16 cs nopw 0x0(%rax,%rax,1)
   0x0000000000413cde <+222>:   xchg   %ax,%ax
   0x0000000000413ce0 <+224>:   tzcnt  %eax,%eax
   0x0000000000413ce4 <+228>:   movzbl (%rsi,%rax,1),%ecx
   0x0000000000413ce8 <+232>:   movzbl (%rdi,%rax,1),%eax
   0x0000000000413cec <+236>:   sub    %ecx,%eax
   0x0000000000413cee <+238>:   vzeroupper
   0x0000000000413cf1 <+241>:   ret
   0x0000000000413cf2 <+242>:   data16 cs nopw 0x0(%rax,%rax,1)
   0x0000000000413cfd <+253>:   nopl   (%rax)
   0x0000000000413d00 <+256>:   tzcnt  %eax,%eax
   0x0000000000413d04 <+260>:   movzbl 0x20(%rsi,%rax,1),%ecx
   0x0000000000413d09 <+265>:   movzbl 0x20(%rdi,%rax,1),%eax
   0x0000000000413d0e <+270>:   sub    %ecx,%eax
   0x0000000000413d10 <+272>:   vzeroupper
   0x0000000000413d13 <+275>:   ret
   0x0000000000413d14 <+276>:   data16 cs nopw 0x0(%rax,%rax,1)
   0x0000000000413d1f <+287>:   nop
   0x0000000000413d20 <+288>:   tzcnt  %eax,%eax
   0x0000000000413d24 <+292>:   movzbl 0x40(%rsi,%rax,1),%ecx
   0x0000000000413d29 <+297>:   movzbl 0x40(%rdi,%rax,1),%eax
   0x0000000000413d2e <+302>:   sub    %ecx,%eax
   0x0000000000413d30 <+304>:   vzeroupper
   0x0000000000413d33 <+307>:   ret
   0x0000000000413d34 <+308>:   data16 cs nopw 0x0(%rax,%rax,1)
   0x0000000000413d3f <+319>:   nop
   0x0000000000413d40 <+320>:   add    %rdi,%rsi
   0x0000000000413d43 <+323>:   vpmovmskb %ymm1,%eax
   0x0000000000413d47 <+327>:   inc    %eax
   0x0000000000413d49 <+329>:   jne    0x413ce0 <__memcmp_avx2_movbe+224>
   0x0000000000413d4b <+331>:   vpmovmskb %ymm2,%eax
   0x0000000000413d4f <+335>:   inc    %eax
   0x0000000000413d51 <+337>:   jne    0x413d00 <__memcmp_avx2_movbe+256>
   0x0000000000413d53 <+339>:   vpmovmskb %ymm3,%eax
   0x0000000000413d57 <+343>:   inc    %eax
   0x0000000000413d59 <+345>:   jne    0x413d20 <__memcmp_avx2_movbe+288>
   0x0000000000413d5b <+347>:   tzcnt  %ecx,%ecx
   0x0000000000413d5f <+351>:   movzbl 0x60(%rdi,%rcx,1),%eax
   0x0000000000413d64 <+356>:   movzbl 0x60(%rsi,%rcx,1),%ecx
   0x0000000000413d69 <+361>:   sub    %ecx,%eax
   0x0000000000413d6b <+363>:   vzeroupper
   0x0000000000413d6e <+366>:   ret
   0x0000000000413d6f <+367>:   nop
   0x0000000000413d70 <+368>:   lea    -0x80(%rdi,%rdx,1),%rdx
   0x0000000000413d75 <+373>:   sub    %rdi,%rsi
   0x0000000000413d78 <+376>:   and    $0xffffffffffffffe0,%rdi
   0x0000000000413d7c <+380>:   sub    $0xffffffffffffff80,%rdi
   0x0000000000413d80 <+384>:   vmovdqu (%rsi,%rdi,1),%ymm1
   0x0000000000413d85 <+389>:   vpcmpeqb (%rdi),%ymm1,%ymm1
   0x0000000000413d89 <+393>:   vmovdqu 0x20(%rsi,%rdi,1),%ymm2
   0x0000000000413d8f <+399>:   vpcmpeqb 0x20(%rdi),%ymm2,%ymm2
   0x0000000000413d94 <+404>:   vmovdqu 0x40(%rsi,%rdi,1),%ymm3
   0x0000000000413d9a <+410>:   vpcmpeqb 0x40(%rdi),%ymm3,%ymm3
   0x0000000000413d9f <+415>:   vmovdqu 0x60(%rsi,%rdi,1),%ymm4
   0x0000000000413da5 <+421>:   vpcmpeqb 0x60(%rdi),%ymm4,%ymm4
   0x0000000000413daa <+426>:   vpand  %ymm1,%ymm2,%ymm5
   0x0000000000413dae <+430>:   vpand  %ymm3,%ymm4,%ymm6
   0x0000000000413db2 <+434>:   vpand  %ymm5,%ymm6,%ymm7
   0x0000000000413db6 <+438>:   vpmovmskb %ymm7,%ecx
   0x0000000000413dba <+442>:   inc    %ecx
   0x0000000000413dbc <+444>:   jne    0x413d40 <__memcmp_avx2_movbe+320>
   0x0000000000413dbe <+446>:   sub    $0xffffffffffffff80,%rdi
   0x0000000000413dc2 <+450>:   cmp    %rdx,%rdi
   0x0000000000413dc5 <+453>:   jb     0x413d80 <__memcmp_avx2_movbe+384>
   0x0000000000413dc7 <+455>:   sub    %rdx,%rdi
   0x0000000000413dca <+458>:   cmp    $0x60,%edi
   0x0000000000413dcd <+461>:   jae    0x413e30 <__memcmp_avx2_movbe+560>
   0x0000000000413dcf <+463>:   vmovdqu 0x40(%rsi,%rdx,1),%ymm3
   0x0000000000413dd5 <+469>:   cmp    $0x40,%edi
   0x0000000000413dd8 <+472>:   jae    0x413e20 <__memcmp_avx2_movbe+544>
   0x0000000000413dda <+474>:   vmovdqu (%rsi,%rdx,1),%ymm1
   0x0000000000413ddf <+479>:   vpcmpeqb (%rdx),%ymm1,%ymm1
   0x0000000000413de3 <+483>:   vmovdqu 0x20(%rsi,%rdx,1),%ymm2
   0x0000000000413de9 <+489>:   vpcmpeqb 0x20(%rdx),%ymm2,%ymm2
   0x0000000000413dee <+494>:   vpcmpeqb 0x40(%rdx),%ymm3,%ymm3
   0x0000000000413df3 <+499>:   vmovdqu 0x60(%rsi,%rdx,1),%ymm4
   0x0000000000413df9 <+505>:   vpcmpeqb 0x60(%rdx),%ymm4,%ymm4
   0x0000000000413dfe <+510>:   vpand  %ymm1,%ymm2,%ymm5
   0x0000000000413e02 <+514>:   vpand  %ymm3,%ymm4,%ymm6
   0x0000000000413e06 <+518>:   vpand  %ymm5,%ymm6,%ymm7
   0x0000000000413e0a <+522>:   vpmovmskb %ymm7,%ecx
   0x0000000000413e0e <+526>:   mov    %rdx,%rdi
   0x0000000000413e11 <+529>:   inc    %ecx
   0x0000000000413e13 <+531>:   jne    0x413d40 <__memcmp_avx2_movbe+320>
   0x0000000000413e19 <+537>:   vzeroupper
   0x0000000000413e1c <+540>:   ret
   0x0000000000413e1d <+541>:   nopl   (%rax)
   0x0000000000413e20 <+544>:   vpcmpeqb 0x40(%rdx),%ymm3,%ymm3
   0x0000000000413e25 <+549>:   vpmovmskb %ymm3,%eax
   0x0000000000413e29 <+553>:   inc    %eax
   0x0000000000413e2b <+555>:   jne    0x413e80 <__memcmp_avx2_movbe+640>
   0x0000000000413e2d <+557>:   nopl   (%rax)
   0x0000000000413e30 <+560>:   vmovdqu 0x60(%rsi,%rdx,1),%ymm4
   0x0000000000413e36 <+566>:   vpcmpeqb 0x60(%rdx),%ymm4,%ymm4
   0x0000000000413e3b <+571>:   vpmovmskb %ymm4,%eax
   0x0000000000413e3f <+575>:   inc    %eax
   0x0000000000413e41 <+577>:   jne    0x413e84 <__memcmp_avx2_movbe+644>
   0x0000000000413e43 <+579>:   vzeroupper
   0x0000000000413e46 <+582>:   ret
   0x0000000000413e47 <+583>:   nopw   0x0(%rax,%rax,1)
   0x0000000000413e50 <+592>:   vmovdqu -0x40(%rsi,%rdx,1),%ymm1
   0x0000000000413e56 <+598>:   vpcmpeqb -0x40(%rdi,%rdx,1),%ymm1,%ymm1
   0x0000000000413e5c <+604>:   vpmovmskb %ymm1,%eax
   0x0000000000413e60 <+608>:   inc    %eax
   0x0000000000413e62 <+610>:   jne    0x413ea0 <__memcmp_avx2_movbe+672>
   0x0000000000413e64 <+612>:   vmovdqu -0x20(%rsi,%rdx,1),%ymm1
   0x0000000000413e6a <+618>:   vpcmpeqb -0x20(%rdi,%rdx,1),%ymm1,%ymm1
   0x0000000000413e70 <+624>:   vpmovmskb %ymm1,%eax
   0x0000000000413e74 <+628>:   inc    %eax
   0x0000000000413e76 <+630>:   jne    0x413ec0 <__memcmp_avx2_movbe+704>
   0x0000000000413e78 <+632>:   vzeroupper
   0x0000000000413e7b <+635>:   ret
   0x0000000000413e7c <+636>:   nopl   0x0(%rax)
   0x0000000000413e80 <+640>:   sub    $0x20,%rdx
   0x0000000000413e84 <+644>:   tzcnt  %eax,%eax
   0x0000000000413e88 <+648>:   add    %rdx,%rax
   0x0000000000413e8b <+651>:   movzbl 0x60(%rsi,%rax,1),%ecx
   0x0000000000413e90 <+656>:   movzbl 0x60(%rax),%eax
   0x0000000000413e94 <+660>:   sub    %ecx,%eax
   0x0000000000413e96 <+662>:   vzeroupper
   0x0000000000413e99 <+665>:   ret
   0x0000000000413e9a <+666>:   nopw   0x0(%rax,%rax,1)
   0x0000000000413ea0 <+672>:   tzcnt  %eax,%eax
   0x0000000000413ea4 <+676>:   add    %edx,%eax
   0x0000000000413ea6 <+678>:   movzbl -0x40(%rsi,%rax,1),%ecx
   0x0000000000413eab <+683>:   movzbl -0x40(%rdi,%rax,1),%eax
   0x0000000000413eb0 <+688>:   sub    %ecx,%eax
   0x0000000000413eb2 <+690>:   vzeroupper
   0x0000000000413eb5 <+693>:   ret
   0x0000000000413eb6 <+694>:   cs nopw 0x0(%rax,%rax,1)
   0x0000000000413ec0 <+704>:   tzcnt  %eax,%eax
   0x0000000000413ec4 <+708>:   add    %edx,%eax
   0x0000000000413ec6 <+710>:   movzbl -0x20(%rsi,%rax,1),%ecx
   0x0000000000413ecb <+715>:   movzbl -0x20(%rdi,%rax,1),%eax
   0x0000000000413ed0 <+720>:   sub    %ecx,%eax
   0x0000000000413ed2 <+722>:   vzeroupper
   0x0000000000413ed5 <+725>:   ret
   0x0000000000413ed6 <+726>:   cs nopw 0x0(%rax,%rax,1)
   0x0000000000413ee0 <+736>:   cmp    $0x1,%edx
   0x0000000000413ee3 <+739>:   jbe    0x413f60 <__memcmp_avx2_movbe+864>
   0x0000000000413ee5 <+741>:   mov    %edi,%eax
   0x0000000000413ee7 <+743>:   or     %esi,%eax
   0x0000000000413ee9 <+745>:   and    $0xfff,%eax
   0x0000000000413eee <+750>:   cmp    $0xfe0,%eax
   0x0000000000413ef3 <+755>:   jg     0x413f20 <__memcmp_avx2_movbe+800>
   0x0000000000413ef5 <+757>:   vmovdqu (%rsi),%ymm2
   0x0000000000413ef9 <+761>:   vpcmpeqb (%rdi),%ymm2,%ymm2
   0x0000000000413efd <+765>:   vpmovmskb %ymm2,%eax
   0x0000000000413f01 <+769>:   inc    %eax
   0x0000000000413f03 <+771>:   bzhi   %edx,%eax,%edx
   0x0000000000413f08 <+776>:   jne    0x413ce0 <__memcmp_avx2_movbe+224>
   0x0000000000413f0e <+782>:   xor    %eax,%eax
   0x0000000000413f10 <+784>:   vzeroupper
   0x0000000000413f13 <+787>:   ret
   0x0000000000413f14 <+788>:   data16 cs nopw 0x0(%rax,%rax,1)
   0x0000000000413f1f <+799>:   nop
   0x0000000000413f20 <+800>:   cmp    $0x10,%edx
   0x0000000000413f23 <+803>:   jae    0x413fa3 <__memcmp_avx2_movbe+931>
   0x0000000000413f25 <+805>:   cmp    $0x8,%edx
   0x0000000000413f28 <+808>:   jae    0x413f80 <__memcmp_avx2_movbe+896>
   0x0000000000413f2a <+810>:   cmp    $0x4,%edx
   0x0000000000413f2d <+813>:   jb     0x413fe0 <__memcmp_avx2_movbe+992>
   0x0000000000413f33 <+819>:   movbe  (%rdi),%eax
   0x0000000000413f37 <+823>:   movbe  (%rsi),%ecx
   0x0000000000413f3b <+827>:   shl    $0x20,%rax
   0x0000000000413f3f <+831>:   shl    $0x20,%rcx
   0x0000000000413f43 <+835>:   movbe  -0x4(%rdi,%rdx,1),%edi
   0x0000000000413f49 <+841>:   movbe  -0x4(%rsi,%rdx,1),%esi
   0x0000000000413f4f <+847>:   or     %rdi,%rax
   0x0000000000413f52 <+850>:   or     %rsi,%rcx
   0x0000000000413f55 <+853>:   sub    %rcx,%rax
   0x0000000000413f58 <+856>:   jne    0x413f70 <__memcmp_avx2_movbe+880>
   0x0000000000413f5a <+858>:   ret
   0x0000000000413f5b <+859>:   nopl   0x0(%rax,%rax,1)
   0x0000000000413f60 <+864>:   jb     0x413f76 <__memcmp_avx2_movbe+886>
   0x0000000000413f62 <+866>:   movzbl (%rsi),%ecx
   0x0000000000413f65 <+869>:   movzbl (%rdi),%eax
   0x0000000000413f68 <+872>:   sub    %ecx,%eax
   0x0000000000413f6a <+874>:   ret
   0x0000000000413f6b <+875>:   nopl   0x0(%rax,%rax,1)
   0x0000000000413f70 <+880>:   sbb    %eax,%eax
   0x0000000000413f72 <+882>:   or     $0x1,%eax
   0x0000000000413f75 <+885>:   ret
   0x0000000000413f76 <+886>:   xor    %eax,%eax
   0x0000000000413f78 <+888>:   ret
   0x0000000000413f79 <+889>:   nopl   0x0(%rax)
   0x0000000000413f80 <+896>:   movbe  (%rdi),%rax
   0x0000000000413f85 <+901>:   movbe  (%rsi),%rcx
   0x0000000000413f8a <+906>:   sub    %rcx,%rax
   0x0000000000413f8d <+909>:   jne    0x413f70 <__memcmp_avx2_movbe+880>
   0x0000000000413f8f <+911>:   movbe  -0x8(%rdi,%rdx,1),%rax
   0x0000000000413f96 <+918>:   movbe  -0x8(%rsi,%rdx,1),%rcx
   0x0000000000413f9d <+925>:   sub    %rcx,%rax
   0x0000000000413fa0 <+928>:   jne    0x413f70 <__memcmp_avx2_movbe+880>
   0x0000000000413fa2 <+930>:   ret
   0x0000000000413fa3 <+931>:   vmovdqu (%rsi),%xmm2
   0x0000000000413fa7 <+935>:   vpcmpeqb (%rdi),%xmm2,%xmm2
   0x0000000000413fab <+939>:   vpmovmskb %xmm2,%eax
   0x0000000000413faf <+943>:   sub    $0xffff,%eax
   0x0000000000413fb4 <+948>:   jne    0x413ce0 <__memcmp_avx2_movbe+224>
   0x0000000000413fba <+954>:   vmovdqu -0x10(%rsi,%rdx,1),%xmm2
   0x0000000000413fc0 <+960>:   lea    -0x10(%rdi,%rdx,1),%rdi
   0x0000000000413fc5 <+965>:   lea    -0x10(%rsi,%rdx,1),%rsi
   0x0000000000413fca <+970>:   vpcmpeqb (%rdi),%xmm2,%xmm2
   0x0000000000413fce <+974>:   vpmovmskb %xmm2,%eax
   0x0000000000413fd2 <+978>:   sub    $0xffff,%eax
   0x0000000000413fd7 <+983>:   jne    0x413ce0 <__memcmp_avx2_movbe+224>
   0x0000000000413fdd <+989>:   ret
   0x0000000000413fde <+990>:   xchg   %ax,%ax
   0x0000000000413fe0 <+992>:   movzwl (%rdi),%eax
   0x0000000000413fe3 <+995>:   movzwl (%rsi),%ecx
   0x0000000000413fe6 <+998>:   bswap  %eax
   0x0000000000413fe8 <+1000>:  bswap  %ecx
   0x0000000000413fea <+1002>:  shr    $1,%eax
   0x0000000000413fec <+1004>:  shr    $1,%ecx
   0x0000000000413fee <+1006>:  movzbl -0x1(%rdi,%rdx,1),%edi
   0x0000000000413ff3 <+1011>:  movzbl -0x1(%rsi,%rdx,1),%esi
   0x0000000000413ff8 <+1016>:  or     %edi,%eax
   0x0000000000413ffa <+1018>:  or     %esi,%ecx
   0x0000000000413ffc <+1020>:  sub    %ecx,%eax
   0x0000000000413ffe <+1022>:  ret
