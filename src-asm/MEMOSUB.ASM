DGROUP                  GROUP    _DATA,_BSS

_DATA                   SEGMENT  WORD PUBLIC 'DATA'
_DATA                   ENDS

_BSS                    SEGMENT  WORD PUBLIC 'BSS'
_BSS                    ENDS

MEMOSUB_TEXT            SEGMENT  BYTE PUBLIC 'CODE'
                        ASSUME   CS:MEMOSUB_TEXT,DS:DGROUP

                        PUBLIC   _MemCopy
                        PUBLIC   _MemOR
                        PUBLIC   _MemSetB
                        PUBLIC   _MemSetW
                        PUBLIC   _MemCmpB
                        PUBLIC   _MemCMPW
                        PUBLIC   _MemSearch
                        PUBLIC   _InPortW
                        PUBLIC   _OutPortW

                        LOCALS

_MemCopy:               push     bp                   ;[bp+ 6] Source Offset
                        mov      bp,sp                ;[bp+ 8] Source Seg
                                                      ;[bp+10] Target Offset
                        push     ds                   ;[bp+12] Target Seg
                        push     di                   ;[bp+14] Size
                        push     si

                        mov      si,[bp+ 6]
                        mov      ds,[bp+ 8]
                        mov      di,[bp+10]
                        mov      es,[bp+12]
                        mov      cx,[bp+14]

                        cmp      cx,0
                        je       @@Exit

                    rep movsb

            @@Exit:     pop      si
                        pop      di
                        pop      ds
                        pop      bp
                        retf

_MemOR:                 push     bp                   ;[bp+ 6] Source Offset
                        mov      bp,sp                ;[bp+ 8] Source Seg
                                                      ;[bp+10] Target Offset
                        push     ds                   ;[bp+12] Target Seg
                        push     di                   ;[bp+14] Size
                        push     si

                        mov      si,[bp+ 6]
                        mov      ds,[bp+ 8]
                        mov      di,[bp+10]
                        mov      es,[bp+12]
                        mov      cx,[bp+14]

                        cmp      cx,0
                        je       @@Exit

               @@Loop:  lodsb
                        or       es:[di],al
                        inc      di
                        loop     @@Loop


              @@Exit:   pop      si
                        pop      di
                        pop      ds
                        pop      bp
                        retf

_MemSetB:               push     bp                   ;[bp+ 6] Target Offset
                        mov      bp,sp                ;[bp+ 8] Target Seg
                                                      ;[bp+10] Data
                        push     di                   ;[bp+12] Size

                        mov      di,[bp+ 6]
                        mov      es,[bp+ 8]
                        mov      cx,[bp+12]
                        mov      ax,[bp+10]

                        cmp      cx,0
                        je       @@Exit

                    rep stosb

             @@Exit:    pop      di
                        pop      bp
                        retf


_MemSetW:               push     bp                   ;[bp+ 6] Target Offset
                        mov      bp,sp                ;[bp+ 8] Target Seg
                                                      ;[bp+10] Data
                        push     di                   ;[bp+12] Size

                        mov      di,[bp+ 6]
                        mov      es,[bp+ 8]
                        mov      cx,[bp+12]
                        mov      ax,[bp+10]

                        cmp      cx,0
                        je       @@Exit

                    rep stosw


            @@Exit:     pop      di
                        pop      bp
                        retf


_MemCmpB:               push     bp                   ;[bp+ 6] Block1 Offset
                        mov      bp,sp                ;[bp+ 8] Block1 Seg
                                                      ;[bp+10] Block2 Offset
                        push     ds                   ;[bp+12] Block2 Seg
                        push     di                   ;[bp+14] Size
                        push     si

                        mov      si,[bp+ 6]
                        mov      ds,[bp+ 8]
                        mov      di,[bp+10]
                        mov      es,[bp+12]
                        mov      cx,[bp+14]

                        cmp      cx,0
                        je       B_Equal

                   repe cmpsb

                        jne      B_NotEqual
         B_Equal:       mov      ax,0
                        jmp      near ptr B_Exit
         B_NotEqual:    jb       B_S2isBigger
         B_S1isBigger:  mov      ax,1
                        jmp      near ptr B_Exit
         B_S2isBigger:  mov      ax,-1

         B_Exit:        pop      si
                        pop      di
                        pop      ds
                        pop      bp
                        retf

_MemCmpW:               push     bp                   ;[bp+ 6] Block1 Offset
                        mov      bp,sp                ;[bp+ 8] Block1 Seg
                                                      ;[bp+10] Block2 Offset
                        push     ds                   ;[bp+12] Block2 Seg
                        push     di                   ;[bp+14] Size
                        push     si

                        mov      si,[bp+ 6]
                        mov      ds,[bp+ 8]
                        mov      di,[bp+10]
                        mov      es,[bp+12]
                        mov      cx,[bp+14]

                        cmp      cx,0
                        je       W_Equal

                   repe cmpsw

                        jne      W_NotEqual
         W_Equal:       mov      ax,0
                        jmp      near ptr W_Exit
         W_NotEqual:    jb       W_S2isBigger
         W_S1isBigger:  mov      ax,1
                        jmp      near ptr W_Exit
         W_S2isBigger:  mov      ax,0FFFFh

         W_Exit:        pop      si
                        pop      di
                        pop      ds
                        pop      bp
                        retf

_MemSearch:             push     bp                   ;[bp+ 6] Target Offset
                        mov      bp,sp                ;[bp+ 8] Target Seg
                                                      ;[bp+10] Data
                        push     di                   ;[bp+12] Size

                        mov      di,[bp+ 6]
                        mov      es,[bp+ 8]
                        mov      cx,[bp+12]
                        mov      ax,[bp+10]

                        cmp      cx,0
                        je       S_NotFound

                        mov      dx,ds

                 repne  scasb
                        jne      S_NotFound
            S_Found:    mov      ax,di
                        dec      ax
                        jmp      near ptr S_Exit
            S_NotFound: xor      ax,ax
                        xor      dx,dx

            S_Exit:     pop      di
                        pop      bp
                        retf

_InPortW:               push     bp
                        mov      bp,sp
                        mov      dx,[bp+6]
                        in       ax,dx
                        pop      bp
                        retf

_OutPortW:              push     bp
                        mov      bp,sp
                        mov      dx,[bp+6]
                        mov      ax,[bp+8]
                        out      dx,ax
                        pop      bp
                        retf

MEMOSUB_TEXT            ENDS

                        END
