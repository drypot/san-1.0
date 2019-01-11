
CGA640                  Segment
                        Assume CS:CGA640,DS:CGA640

                        org    100h

StartUp:                int    20h

                        DW 79                    ; Max Width
                        DW 24                    ; Max Highth
       InverseF         DW 0
                        DW 7 dup (0)

                        DW Offset    InitHanDrv
                        DW Offset    GrMode
                        DW Offset    TextMode
                        DW Offset    ClrScr
                        DW Offset    DrawEng
                        DW Offset    DrawHan
                        DW Offset    DrawEngCur
                        DW Offset    DrawHanCur
                        DW Offset    ScrollUp
                        DW Offset    ScrollDown
                        DW Offset    SaveScreen
                        DW Offset    RestoreScreen
                        DW Offset    SetInverse
                        DW Offset    ClrBox
                        DW Offset    DrawShadowFull
                        DW Offset    DrawShadowHalf
                        DW Offset    DrawShadowHalfDown

                        DW 13 dup (0)

       HanFontPtr       DD 0
       EngFontPtr       DD 0
       GrfFontPtr       DD 0
       SetHanjaImg      DD 0

       MulTable140      dw 0000h,0140h,0280h,03C0h,0500h,0640h,0780h,08C0h
                        dw 0A00h,0B40h,0C80h,0DC0h,0F00h,1040h,1180h,12C0h
                        dw 1400h,1540h,1680h,17C0h,1900h,1A40h,1B80h,1CC0h
                        dw 1E00h


       FirstIndex       db  0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
                        db 15,16,17,18,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

       MidIndex         db  0, 0, 0, 1, 2, 3, 4, 5, 0, 0, 6, 7, 8, 9,10,11
                        db  0, 0,12,13,14,15,16,17, 0, 0,18,19,20,21, 0, 0

       TailIndex        db  0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14
                        db 15,16, 0,17,18,19,20,21,22,23,24,25,26,27, 0, 0

       HanMulTable      dw 0000h,02C0h,0580h,0840h,0B00h,0DC0h,1080h,1340h
                        dw 1600h,18C0h,1B80h,1E40h,2100h,23C0h,2680h,2940h
                        dw 2C00h,2EC0h,3180h,3440h

       TailGroupIndex   dw 6E00h+360h*0,6E00h+360h*0,6E00h+360h*2,6E00h+360h*0
                        dw 6E00h+360h*2,6E00h+360h*1,6E00h+360h*2,6E00h+360h*1
                        dw 6E00h+360h*2,6E00h+360h*3,6E00h+360h*0,6E00h+360h*2
                        dw 6E00h+360h*1,6E00h+360h*3,6E00h+360h*3,6E00h+360h*1
                        dw 6E00h+360h*2,6E00h+360h*1,6E00h+360h*3,6E00h+360h*3
                        dw 6E00h+360h*1,6E00h+360h*1

;       TailGroupIndex   dw 7820h,6E00h,74C0h,6E00h,74C0h,7160h,74C0h,7160h
;                        dw 74C0h,7820h,6E00h,74C0h,6E00h,7820h,7820h,7160h
;                        dw 74C0h,6E00h,7820h,7820h,6E00h,6E00h

       LineIndex        dw 0000h,2000h,0050h,2050h,00A0h,20A0h,00F0h,20F0h
                        dw 0140h,2140h,0190h,2190h,01E0h,21E0h,0230h,2230h
                        dw 0280h,2280h,02D0h,22D0h,0320h,2320h,0370h,2370h
                        dw 03C0h,23C0h,0410h,2410h,0460h,2460h,04B0h,24B0h
                        dw 0500h,2500h,0550h,2550h,05A0h,25A0h,05F0h,25F0h
                        dw 0640h,2640h,0690h,2690h,06E0h,26E0h,0730h,2730h
                        dw 0780h,2780h,07D0h,27D0h,0820h,2820h,0870h,2870h
                        dw 08C0h,28C0h,0910h,2910h,0960h,2960h,09B0h,29B0h
                        dw 0A00h,2A00h,0A50h,2A50h,0AA0h,2AA0h,0AF0h,2AF0h
                        dw 0B40h,2B40h,0B90h,2B90h,0BE0h,2BE0h,0C30h,2C30h
                        dw 0C80h,2C80h,0CD0h,2CD0h,0D20h,2D20h,0D70h,2D70h
                        dw 0DC0h,2DC0h,0E10h,2E10h,0E60h,2E60h,0EB0h,2EB0h
                        dw 0F00h,2F00h,0F50h,2F50h,0FA0h,2FA0h,0FF0h,2FF0h
                        dw 1040h,3040h,1090h,3090h,10E0h,30E0h,1130h,3130h
                        dw 1180h,3180h,11D0h,31D0h,1220h,3220h,1270h,3270h
                        dw 12C0h,32C0h,1310h,3310h,1360h,3360h,13B0h,33B0h
                        dw 1400h,3400h,1450h,3450h,14A0h,34A0h,14F0h,34F0h
                        dw 1540h,3540h,1590h,3590h,15E0h,35E0h,1630h,3630h
                        dw 1680h,3680h,16D0h,36D0h,1720h,3720h,1770h,3770h
                        dw 17C0h,37C0h,1810h,3810h,1860h,3860h,18B0h,38B0h
                        dw 1900h,3900h,1950h,3950h,19A0h,39A0h,19F0h,39F0h
                        dw 1A40h,3A40h,1A90h,3A90h,1AE0h,3AE0h,1B30h,3B30h
                        dw 1B80h,3B80h,1BD0h,3BD0h,1C20h,3C20h,1C70h,3C70h
                        dw 1CC0h,3CC0h,1D10h,3D10h,1D60h,3D60h,1DB0h,3DB0h
                        dw 1E00h,3E00h,1E50h,3E50h,1EA0h,3EA0h,1EF0h,3EF0h

                        LOCALS

InitHanDrv:             retf

GrMode:                 mov     ax,0006h
                        int     10h
                        mov     ax,0500h
                        int     10h
                        retf

TextMode:               mov     ax,0003h
                        int     10h
                        retf

ClrScr:                 push     di

                        mov      ax,0b800h
                        mov      es,ax
                        xor      di,di
                        mov      cx,2000h
                        xor      ax,ax
                        rep      stosw

                        pop      di
                        retf

DrawEng:                push     bp                   ; [bp+6]  x position
                        mov      bp,sp                ; [bp+8]  y position
                        push     si                   ; [bp+10] code
                        push     ds                   ;  ds:si  font_pointer
                                                      ;  es:bx  screen pointer

                        mov      ds,word ptr cs:[EngFontPtr+2]
                        mov      si,[bp+10]
                        mov      cl,4
                        shl      si,cl
                        add      si,word ptr cs:[EngFontPtr]

                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,cs:[bx+offset MulTable140]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      cs:InverseF,1
                        mov      ax,0b800h
                        mov      es,ax
                        je       @@Inverse

                        lodsb                         ; 1
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 3
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 5
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 7
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 9
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 11
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 13
                        or       al,[si]
                        inc      si
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 15
                        or       al,[si]
                        mov      es:[bx],al

                        pop      ds
                        pop      si
                        pop      bp
                        retf

       @@Inverse:       lodsb                         ; 1
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 3
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 5
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 7
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 9
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 11
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,1fb0h
                        lodsb                         ; 13
                        or       al,[si]
                        inc      si
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 15
                        or       al,[si]
                        xor      al,0FFh
                        mov      es:[bx],al

                        pop      ds
                        pop      si
                        pop      bp
                        retf

DrawSpecial:            mov      cl,5
                        shl      bx,cl
                        add      si,bx                ;  ds:si  font_pointer
                                                      ;  es:bx  screen pointer
                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,[bx+offset MulTable140]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      InverseF,1
                        mov      ax,0b800h
                        mov      es,ax
                        lds      ax,GrfFontPtr
                        add      si,ax
                        jmp      near ptr DrawLarge

DrawHan:                push     bp                   ; [bp+ 6] x position
                        mov      bp,sp                ; [bp+ 8] y position
                        push     si                   ; [bp+10] code to put
                        push     ds

			mov      dx,[bp+10]
			cmp      dl,0E0h
			jb	 Grf1?
                        cmp      word ptr cs:[SethanjaImg],0
                        je       Grf1?
                        push     cs
                        pop      es
			call     es:[offset SetHanjaImg]
	                mov	 si,ax
                        mov	 ds,dx
                        jmp      near ptr DrawLarge

	       Grf1?:   mov      bx,cs
			mov      ds,bx
			xor      bx,bx

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
                        jmp      DrawSpecial

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
                        jmp      near ptr DrawSpecial

                n10:    cmp      dl,0DAh
                        jne      n11
                        cmp      bx,187
                        ja       Space
                        mov      si,9184
                        jmp      near ptr DrawSpecial

                n11:    cmp      dl,0DBh
                        jne      n12
                        cmp      bx,161
                        ja       Space
                        mov      si,15200
                        jmp      near ptr DrawSpecial

                n12:    cmp      dl,0DCh
                        jne      n13
                        cmp      bx,187
                        ja       Space
                        mov      si,20384
                        jmp      near ptr DrawSpecial

                n13:    cmp      dl,0DDh
                        jne      n14
                        cmp      bx,176
                        ja       Space
                        mov      si,26400
                        jmp      near ptr DrawSpecial

                n14:    cmp      dl,0DEh
                        cmp      bx,174
                        ja       Space
                        mov      si,32064
                        jmp      near ptr DrawSpecial

                Space:  xor      si,si
                        mov      bx,1143
                        jmp      near ptr DrawSpecial


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
                        mov      si,word ptr [HanFontPtr]
                        add      si,[bx+HanMulTable]
                        mov      bl,al
                        mov      cl,5
                        shl      ax,cl
                        add      si,ax
                        or       dh,dh
                        jnz      ExistTail
                        mov      ds,word ptr [HanFontPtr+2]
                        jmp      near ptr DrawLarge

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
                        add      di,word ptr [HanFontPtr]
                        add      di,2*8

                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,[bx+offset MulTable140]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      InverseF,1
                        mov      ax,0b800h
                        mov      es,ax
                        mov      ds,word ptr [HanFontPtr+2]
                        jne      HanNormal
                        jmp      HanInverse

             HanNormal: lodsw                         ; 1
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 3
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        sub      bx,1FB0h
                        lodsw                         ; 5
                        or       ax,[si]
                        add      si,2
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        or       ax,[si]
                        add      si,2
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 7
                        or       ax,[si]
                        add      si,2
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        or       ax,[si]
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        mov      es:[bx],ax

                        pop      di
                        pop      ds
                        pop      si
                        pop      bp
                        retf

         HanInverse:    lodsw                         ; 1
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 3
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1FB0h
                        lodsw                         ; 5
                        or       ax,[si]
                        inc      si
                        inc      si
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        or       ax,[si]
                        inc      si
                        inc      si
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 7
                        or       ax,[si]
                        inc      si
                        inc      si
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        or       ax,[si]
                        or       ax,[di]
                        add      di,2
                        or       ax,[di]
                        xor      ax,0FFFFh
                        mov      es:[bx],ax


                        pop      di
                        pop      ds
                        pop      si
                        pop      bp
                        retf


                                             ; [bp+6]  x position
                                             ; [bp+8]  y position
                                             ; [bp+10] code
DrawLarge:                                   ;  ds:si  font_pointer
                                             ;  es:bx  screen pointer

                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,cs:[bx+offset MulTable140]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      cs:InverseF,0
                        mov      ax,0b800h
                        mov      es,ax
                        je       DLNormal
                        jmp      near ptr DLInverse

           DLNormal:    lodsw                         ; 1
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 3
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        sub      bx,1FB0h
                        lodsw                         ; 5
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 7
                        or       ax,[si]
                        add      si,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        or       ax,[si]
                        mov      es:[bx],ax

                        pop      ds
                        pop      si
                        pop      bp
                        retf

         DLInverse:     lodsw                         ; 1
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 3
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1FB0h
                        lodsw                         ; 5
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,1fb0h
                        lodsw                         ; 7
                        or       ax,[si]
                        inc      si
                        inc      si
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        or       ax,[si]
                        xor      ax,0FFFFh
                        mov      es:[bx],ax

                        pop      ds
                        pop      si
                        pop      bp
                        retf

DrawEngCur:             push     bp                   ; [bp+8]  y position
                        mov      bp,sp                ; [bp+6]  x position
                        push     di
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,word ptr [bp+8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cx,2
                        add      bx,6*2

           @@Loop:      mov      di,cs:[bx+LineIndex]
                        add      di,[bp+6]
                        xor      byte ptr es:[di],0FFh
                        add      bx,2
                        loop     @@Loop

                        pop      di
                        pop      bp
                        retf

DrawHanCur:             push     bp                   ; [bp+8]  y position
                        mov      bp,sp                ; [bp+6]  x position
                        push     di
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,word ptr [bp+8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cx,8

           @@Loop:      mov      di,cs:[bx+LineIndex]
                        add      di,[bp+6]
                        xor      byte ptr es:[di],0FFh
                        add      bx,2
                        loop     @@Loop

                        pop      di
                        pop      bp
                        retf

ScrollUp:               push     bp
                        mov      bp,sp
                        push     ds
                        push     si
                        push     di
                                                      ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      ds,ax

                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        shr      dx,1

          @@Loop:       mov      di,cs:[LineIndex+bx]
                        add      di,[bp+6]
                        mov      si,cs:[LineIndex+32+bx]
                        add      si,[bp+6]
                        mov      cx,dx
                        rep      movsw
                        add      bx,2
                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      si
                        pop      ds
                        pop      bp
                        retf

ScrollDown:             push     bp
                        mov      bp,sp
                        push     ds
                        push     si
                        push     di
                                                      ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      ds,ax

                        mov      bx,[bp+12]
                        mov      cl,4
                        shl      bx,cl
                        sub      bx,2
                        mov      cl,4
                        shl      word ptr [bp+8],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        shr      dx,1

          @@Loop:       mov      si,cs:[LineIndex+bx]
                        add      si,[bp+6]
                        mov      di,cs:[LineIndex+32+bx]
                        add      di,[bp+6]
                        mov      cx,dx
                        rep      movsw
                        cmp      bx,0
                        je       @@exit
                        sub      bx,2
                        cmp      bx,[bp+8]
                        jae      @@Loop

            @@exit:     pop      di
                        pop      si
                        pop      ds
                        pop      bp
                        retf

SaveScreen:             push     bp
                        mov      bp,sp
                        push     ds
                        push     si
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                                                      ;[bp+14] Offset To Save
                                                      ;[bp+16] Seg To Save
                        mov      es,[bp+16]
                        mov      di,[bp+14]
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        mov      ax,0b800h
                        mov      ds,ax

          @@Loop:       mov      si,word ptr cs:[bx+LineIndex]
                        add      si,[bp+6]
                        mov      cx,dx
                        rep      movsb
                        add      bx,2
                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      si
                        pop      ds
                        pop      bp
                        retf

RestoreScreen:          push     bp
                        mov      bp,sp
                        push     ds
                        push     si
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                                                      ;[bp+14] Offset To Res
                                                      ;[bp+16] Seg To Res
                        mov      si,[bp+14]
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        mov      ds,[bp+16]
          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]
                        mov      cx,dx
                        rep      movsb
                        add      bx,2
                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      si
                        pop      ds
                        pop      bp
                        retf

SetInverse:             push     bp
                        mov      bp,sp
                        push     ds
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        mov      ds,ax

          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]
                        mov      cx,dx

            @@LoopSub:  mov      al,[di]
                        xor      al,0FFh
                        mov      [di],al
                        inc      di
                        loop     @@LoopSub

                        add      bx,2
                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      ds
                        pop      bp
                        retf

ClrBox:                 push     bp
                        mov      bp,sp
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        xor      al,al
                        cmp      cs:[InverseF],0
                        je       @@Loop
                        mov      al,0FFh

          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]
                        mov      cx,dx
                        rep      stosb
                        add      bx,2
                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      bp
                        retf

DrawShadowFull:         push     bp
                        mov      bp,sp
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx

          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop2:      mov      al,es:[di]
                        and      al,0AAh
                        stosb
                        loop     @@Loop2
                        add      bx,2

                        mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop3:      mov      al,es:[di]
                        and      al,55h
                        stosb
                        loop     @@Loop3
                        add      bx,2

                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      bp
                        retf

DrawShadowHalf:         push     bp
                        mov      bp,sp
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        sub      word ptr [bp+12],16
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx

          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop2:      mov      al,es:[di]
                        and      al,0AAh
                        stosb
                        loop     @@Loop2
                        add      bx,2

                        mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop3:      mov      al,es:[di]
                        and      al,55h
                        stosb
                        loop     @@Loop3
                        add      bx,2

                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      bp
                        retf

DrawShadowHalfDown:     push     bp
                        mov      bp,sp
                        push     di                   ;[bp+ 6] X1
                                                      ;[bp+ 8] Y1
                                                      ;[bp+10] X2
                                                      ;[bp+12] Y2
                        mov      ax,0b800h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,4
                        shl      bx,cl
                        mov      cl,4
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        add      bx,16
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx

          @@Loop:       mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop2:      mov      al,es:[di]
                        and      al,0AAh
                        stosb
                        loop     @@Loop2
                        add      bx,2

                        mov      di,word ptr cs:[bx+LineIndex]
                        add      di,[bp+6]

                        mov      cx,dx
          @@Loop3:      mov      al,es:[di]
                        and      al,55h
                        stosb
                        loop     @@Loop3
                        add      bx,2

                        cmp      bx,[bp+12]
                        jb       @@Loop

                        pop      di
                        pop      bp
                        retf

CGA640                  Ends

                        End      StartUp
