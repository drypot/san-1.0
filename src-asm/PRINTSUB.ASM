DGROUP                  GROUP    _DATA,_BSS

_DATA                   SEGMENT  WORD PUBLIC 'DATA'

                        Extrn    _HanFont
                        Extrn    _GrfFont
                        Extrn    _HanjaFileHandler

       FirstIndex       db  0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
                        db 15,16,17,18,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

       MidIndex         db  0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 6, 7, 8, 9,10,11
                        db  0, 0,12,13,14,15,16,17, 0, 0,18,19,20,21, 0, 0

       TailIndex        db  0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
                        db 15,16, 0,17,18,19,20,21,22,23,24,25,26,27, 0, 0

       HanMulTable      dw 0000h,02C0h,0580h,0840h,0B00h,0DC0h,1080h,1340h
                        dw 1600h,18C0h,1B80h,1E40h,2100h,23C0h,2680h,2940h
                        dw 2C00h,2EC0h,3180h,3440h

       TailGroupIndex   dw 7820h,6E00h,74C0h,6E00h,74C0h,7160h,74C0h,7160h
                        dw 74C0h,7820h,6E00h,74C0h,6E00h,7820h,7820h,7160h
                        dw 74C0h,6E00h,7820h,7820h,6E00h,6E00h

_DATA                   ENDS

_BSS                    SEGMENT  WORD PUBLIC 'BSS'
_BSS                    ENDS

                        extrn    _HanjaPos:FAR

PrintSub_TEXT           SEGMENT  BYTE PUBLIC 'CODE'
                        ASSUME   CS:PrintSub_TEXT,DS:DGROUP

                        Public   _GetHanFontPos

                        LOCALS


Special:                mov      cl,5
                        shl      bx,cl
                        add      si,bx                ;  ds:si  font_pointer
                                                      ;  es:bx  screen pointer
                        lds      ax,dword ptr _GrfFont
                        add      si,ax
                        mov      ax,si
                        mov      bx,0
                        mov      dx,ds
                        jmp      near ptr Return

_GetHanFontPos:         push     bp
                        mov      bp,sp
                        push     si
                        push     ds


                        mov      dx,[bp+6]


			cmp      dl,0E0h
			jb	 Grf1?
                        cmp      word ptr _HanjaFileHandler,0ffffh
                        je       Grf1?
			call     _HanjaPos
                        mov      bx,0
                        jmp      near ptr Return

               Grf1?:   xor      bx,bx

                        cmp      dl,0D4h              ; D4 ‹aœÏ¢ ¢…¸a·¥ˆa?
                        jne      Grf2?
                        cmp      dh,80h
                        jae      Grf1
                        jmp      near ptr Start_HanGul

                Grf1:   jne      g5
                        jmp      near ptr Space
                  g5:   cmp      dh,0feh
                        jb       g4
                        jmp      near ptr Space
                  g4:   xor      si,si
                        mov      bl,dh
                        sub      bx,81h
                        jmp      Special

              Grf2?:    cmp      dl,0D9h              ; D9--DE ‹aœÏ¢ ¢…¸a?
                        jae      g1
                        jmp      near ptr Start_HanGul
                  g1:   cmp      dl,0DEh
                        jbe      g2
                        jmp      near ptr Start_HanGul
                  g2:   cmp      dh,30h
                        jae      g3
                        jmp      near ptr Start_HanGul

                                                     ; ‹aœý”a¡e..
                  g3:   mov      bl,dh
                        sub      bx,31h
                        cmp      bx,07Fh-31h
                        jb       n4
                        cmp      bx,090h-31h
                        ja       n3
                        jmp      Space
                 n3:    sub      bx,12h

                 n4:
                        cmp      dl,0D9h
                        jne      n10
                        cmp      bx,0
                        je       Space
                        dec      bx
                        cmp      bx,161
                        ja       Space
                        mov      si,4000
                        jmp      near ptr Special

                n10:    cmp      dl,0DAh
                        jne      n11
                        cmp      bx,187
                        ja       Space
                        mov      si,9184
                        jmp      near ptr Special

                n11:    cmp      dl,0DBh
                        jne      n12
                        cmp      bx,161
                        ja       Space
                        mov      si,15200
                        jmp      near ptr Special

                n12:    cmp      dl,0DCh
                        jne      n13
                        cmp      bx,187
                        ja       Space
                        mov      si,20384
                        jmp      near ptr Special

                n13:    cmp      dl,0DDh
                        jne      n14
                        cmp      bx,176
                        ja       Space
                        mov      si,26400
                        jmp      near ptr Special

                n14:    cmp      dl,0DEh
                        cmp      bx,174
                        ja       Space
                        mov      si,32064
                        jmp      near ptr Special

                Space:  xor      si,si
                        mov      bx,1143
                        jmp      near ptr Special


          Start_HanGul:

                        mov      cl,3                 ; mid
                        mov      ax,dx
                        xchg     ah,al
                        shl      ax,cl
                        and      ah,1fh
                        mov      bl,ah
                        mov      ah,byte ptr [bx+MidIndex]
                        mov      al,ah
                        xor      ah,ah

                        mov      bl,dh                ; tail
                        and      bl,1fh
                        mov      dh,byte ptr [bx+offset TailIndex]

                        and      dl,7fh               ;  first
                        mov      bl,dl
                        mov      cl,2
                        shr      bl,cl
                        mov      dl,byte ptr [bx+offset FirstIndex]
                        mov      bl,dl

                                 ;al  mid
                                 ;bl  first
                                 ;dh  tail

                        shl      bx,1
                        mov      si,word ptr [_HanFont]
                        add      si,[bx+HanMulTable]
                        mov      bl,al
                        mov      cl,5
                        shl      ax,cl
                        add      si,ax
                        or       dh,dh
                        jnz      ExistTail
                        mov      ax,si
                        mov      bx,0
                        mov      dx,word ptr [_HanFont+2]
                        jmp      near ptr Return

              ExistTail:push     di
                        add      si,3700h

                        xor      cx,cx
                        mov      cl,dh
                        mov      di,cx
                        dec      di
                        mov      cl,5
                        shl      di,cl
                        xor      bh,bh
                        shl      bx,1
                        add      di,word ptr [bx+ TailGroupIndex]
                        add      di,word ptr [_HanFont]
                        mov      ax,si
                        mov      bx,di
                        mov      dx,word ptr [_HanFont+2]
                        pop      di

         Return:        pop      ds
                        pop      si
                        pop      bp
                        retf

PrintSub_TEXT           ENDS

                        END
