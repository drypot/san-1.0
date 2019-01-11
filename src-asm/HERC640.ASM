
Herc640                 Segment
                        Assume CS:Herc640,DS:Herc640

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

       TextModeSetting  db 61h,50h,52h,0Fh,19h,06h,19h,19h,02h,0Dh,0Bh,0Ch,28h
       GrMode640_0      db 31h,28h,29h,08h,68h,02h,64h,65h,02h,03h,02h,01h,1Eh

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


       LineIndex        dw 0000h,2000h,4000h,6000h,0050h,2050h,4050h,6050h
                        dw 00A0h,20A0h,40A0h,60A0h,00F0h,20F0h,40F0h,60F0h
                        dw 0140h,2140h,4140h,6140h,0190h,2190h,4190h,6190h
                        dw 01E0h,21E0h,41E0h,61E0h,0230h,2230h,4230h,6230h
                        dw 0280h,2280h,4280h,6280h,02D0h,22D0h,42D0h,62D0h
                        dw 0320h,2320h,4320h,6320h,0370h,2370h,4370h,6370h
                        dw 03C0h,23C0h,43C0h,63C0h,0410h,2410h,4410h,6410h
                        dw 0460h,2460h,4460h,6460h,04B0h,24B0h,44B0h,64B0h
                        dw 0500h,2500h,4500h,6500h,0550h,2550h,4550h,6550h
                        dw 05A0h,25A0h,45A0h,65A0h,05F0h,25F0h,45F0h,65F0h
                        dw 0640h,2640h,4640h,6640h,0690h,2690h,4690h,6690h
                        dw 06E0h,26E0h,46E0h,66E0h,0730h,2730h,4730h,6730h
                        dw 0780h,2780h,4780h,6780h,07D0h,27D0h,47D0h,67D0h
                        dw 0820h,2820h,4820h,6820h,0870h,2870h,4870h,6870h
                        dw 08C0h,28C0h,48C0h,68C0h,0910h,2910h,4910h,6910h
                        dw 0960h,2960h,4960h,6960h,09B0h,29B0h,49B0h,69B0h
                        dw 0A00h,2A00h,4A00h,6A00h,0A50h,2A50h,4A50h,6A50h
                        dw 0AA0h,2AA0h,4AA0h,6AA0h,0AF0h,2AF0h,4AF0h,6AF0h
                        dw 0B40h,2B40h,4B40h,6B40h,0B90h,2B90h,4B90h,6B90h
                        dw 0BE0h,2BE0h,4BE0h,6BE0h,0C30h,2C30h,4C30h,6C30h
                        dw 0C80h,2C80h,4C80h,6C80h,0CD0h,2CD0h,4CD0h,6CD0h
                        dw 0D20h,2D20h,4D20h,6D20h,0D70h,2D70h,4D70h,6D70h
                        dw 0DC0h,2DC0h,4DC0h,6DC0h,0E10h,2E10h,4E10h,6E10h
                        dw 0E60h,2E60h,4E60h,6E60h,0EB0h,2EB0h,4EB0h,6EB0h
                        dw 0F00h,2F00h,4F00h,6F00h,0F50h,2F50h,4F50h,6F50h
                        dw 0FA0h,2FA0h,4FA0h,6FA0h,0FF0h,2FF0h,4FF0h,6FF0h
                        dw 1040h,3040h,5040h,7040h,1090h,3090h,5090h,7090h
                        dw 10E0h,30E0h,50E0h,70E0h,1130h,3130h,5130h,7130h
                        dw 1180h,3180h,5180h,7180h,11D0h,31D0h,51D0h,71D0h
                        dw 1220h,3220h,5220h,7220h,1270h,3270h,5270h,7270h
                        dw 12C0h,32C0h,52C0h,72C0h,1310h,3310h,5310h,7310h
                        dw 1360h,3360h,5360h,7360h,13B0h,33B0h,53B0h,73B0h
                        dw 1400h,3400h,5400h,7400h,1450h,3450h,5450h,7450h
                        dw 14A0h,34A0h,54A0h,74A0h,14F0h,34F0h,54F0h,74F0h
                        dw 1540h,3540h,5540h,7540h,1590h,3590h,5590h,7590h
                        dw 15E0h,35E0h,55E0h,75E0h,1630h,3630h,5630h,7630h
                        dw 1680h,3680h,5680h,7680h,16D0h,36D0h,56D0h,76D0h
                        dw 1720h,3720h,5720h,7720h,1770h,3770h,5770h,7770h
                        dw 17C0h,37C0h,57C0h,77C0h,1810h,3810h,5810h,7810h
                        dw 1860h,3860h,5860h,7860h,18B0h,38B0h,58B0h,78B0h
                        dw 1900h,3900h,5900h,7900h,1950h,3950h,5950h,7950h
                        dw 19A0h,39A0h,59A0h,79A0h,19F0h,39F0h,59F0h,79F0h
                        dw 1A40h,3A40h,5A40h,7A40h,1A90h,3A90h,5A90h,7A90h
                        dw 1AE0h,3AE0h,5AE0h,7AE0h,1B30h,3B30h,5B30h,7B30h
                        dw 1B80h,3B80h,5B80h,7B80h,1BD0h,3BD0h,5BD0h,7BD0h
                        dw 1C20h,3C20h,5C20h,7C20h,1C70h,3C70h,5C70h,7C70h
                        dw 1CC0h,3CC0h,5CC0h,7CC0h,1D10h,3D10h,5D10h,7D10h
                        dw 1D60h,3D60h,5D60h,7D60h,1DB0h,3DB0h,5DB0h,7DB0h
                        dw 1E00h,3E00h,5E00h,7E00h,1E50h,3E50h,5E50h,7E50h
                        dw 1EA0h,3EA0h,5EA0h,7EA0h,1EF0h,3EF0h,5EF0h,7EF0h

                        LOCALS

InitHanDrv:             retf

Wait_Retrace:           mov      dx,3BAh
       InRetrace:       in       al,dx
                        shl      al,1
                        jnc      InRetrace
       NotInRetrace:    in       al,dx
                        shl      al,1
                        jc       NotInRetrace
                        ret

GrMode:                 push    bp
                        mov     bp,sp
                        push    si
                        push    di
                        push    ds

                        mov     ax,cs
                        mov     ds,ax
                        mov     si,offset GrMode640_0

       TurnMode:        call    Wait_Retrace
                        mov     dx,03BFh             ;Full Mode
                        mov     al,3
                        out     dx,al
                        mov     dl,0B8h
                        mov     al,0
                        out     dx,al

                        mov     dl,0b4h
                        mov     al,0
       @@Loop:          mov     bl,al
                        out     dx,al
                        inc     dl
                        lodsb
                        out     dx,al
                        dec     dl
                        mov     al,bl
                        inc     al
                        cmp     al,0Ch
                        jb      @@Loop
                        add     dl,4
                        lodsb
                        out     dx,al

                        pop     ds
                        pop     di
                        pop     si
                        pop     bp
                        retf

TextMode:               push    si
                        push    di
                        push    ds

                        mov     ax,cs
                        mov     ds,ax
                        call    Wait_Retrace
                        mov     si,offset TextModeSetting
                        mov     dx,03BFh             ;Full Mode
                        mov     al,3
                        out     dx,al
                        mov     dl,0B8h
                        mov     al,0
                        out     dx,al
                        mov     dl,0b4h
                        mov     al,0
       @@Loop:          mov     bl,al
                        out     dx,al
                        inc     dl
                        lodsb
                        out     dx,al
                        dec     dl
                        mov     al,bl
                        inc     al
                        cmp     al,0Ch
                        jb      @@Loop
                        add     dl,4
                        lodsb
                        out     dx,al

                        pop     ds
                        pop     di
                        pop     si
                        retf

ClrScr:                 push     di

                        mov      ax,0b000h
                        mov      es,ax
                        xor      di,di
                        mov      cx,4000h
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
                        mov      ax,0B000h
                        mov      es,ax
                        je       @@Inverse

                        lodsb                         ; 1
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 2
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 3
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 4
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 5
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 6
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 7
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 8
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 9
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 10
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 11
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 12
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 13
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 14
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 15
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 16
                        mov      es:[bx],al

                        pop      ds
                        pop      si
                        pop      bp
                        retf

       @@Inverse:       lodsb                         ; 1
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 2
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 3
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 4
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 5
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 6
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 7
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 8
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 9
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 10
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 11
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 12
                        xor      al,0FFh
                        mov      es:[bx],al
                        sub      bx,5FB0h
                        lodsb                         ; 13
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 14
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 15
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bh,20h
                        lodsb                         ; 16
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
                        mov      ax,0B000h
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      ds,word ptr [HanFontPtr+2]
                        jne      HanNormal
                        jmp      HanInverse

             HanNormal: lodsw                         ; 1
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 3
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 5
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 7
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 9
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 10
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 11
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 12
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 13
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 14
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 15
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 16
                        or       ax,[di]
                        mov      es:[bx],ax

                        pop      di
                        pop      ds
                        pop      si
                        pop      bp
                        retf

         HanInverse:    lodsw                         ; 1
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 3
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 5
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 7
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 9
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 10
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 11
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 12
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 13
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 14
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 15
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 16
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
                        mov      ax,0B000h
                        mov      es,ax
                        je       DLNormal
                        jmp      near ptr DLInverse

           DLNormal:    lodsw                         ; 1
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 3
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 5
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 7
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 9
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 10
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 11
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 12
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 13
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 14
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 15
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 16
                        mov      es:[bx],ax

                        pop      ds
                        pop      si
                        pop      bp
                        retf

         DLInverse:     lodsw                         ; 1
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 3
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 4
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 5
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 6
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 7
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 8
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 9
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 10
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 11
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 12
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        sub      bx,5FB0h
                        lodsw                         ; 13
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 14
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 15
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bh,20h
                        lodsw                         ; 16
                        xor      ax,0FFFFh
                        mov      es:[bx],ax

                        pop      ds
                        pop      si
                        pop      bp
                        retf

DrawEngCur:             push     bp                   ; [bp+8]  y position
                        mov      bp,sp                ; [bp+6]  x position
                        push     di
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,word ptr [bp+8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cx,2
                        add      bx,14*2

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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,word ptr [bp+8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cx,16

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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      ds,ax

                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      ds,ax

                        mov      bx,[bp+12]
                        mov      cl,5
                        shl      bx,cl
                        sub      bx,2
                        mov      cl,5
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
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],cl
                        mov      dx,[bp+10]
                        sub      dx,[bp+ 6]
                        inc      dx
                        mov      ax,0B000h
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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
                        mov      ax,0B000h
                        mov      es,ax
                        mov      bx,[bp+ 8]
                        mov      cl,5
                        shl      bx,cl
                        mov      cl,5
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

Herc640                 Ends

                        End      StartUp
