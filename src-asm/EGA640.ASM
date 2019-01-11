
ega640                  Segment
                        Assume CS:ega640,DS:ega640

                        org    100h

StartUp:                int    20h

                        DW 79                    ; Max Width
                        DW 21                    ; Max Highth
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

       ColorTable       db 01h
                        db 3fh,02h,00h,04h,00h,00h,00h,08h
                        db 00h,00h,00h,00h,00h,00h,3fh,00h

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

       MulTable500      dw 0000h,0500h,0A00h,0F00h,1400h,1900h,1E00h,2300h
                        dw 2800h,2D00h,3200h,3700h,3C00h,4100h,4600h,4B00h
                        dw 5000h,5500h,5A00h,5F00h,6400h,6900h,6E00h,7300h
                        dw 7800h,7D00h,8200h,8700h,8C00h,9100h

       LineIndex        dw 0000h,0050h,00A0h,00F0h,0140h,0190h,01E0h,0230h
                        dw 0280h,02D0h,0320h,0370h,03C0h,0410h,0460h,04B0h
                        dw 0500h,0550h,05A0h,05F0h,0640h,0690h,06E0h,0730h
                        dw 0780h,07D0h,0820h,0870h,08C0h,0910h,0960h,09B0h
                        dw 0A00h,0A50h,0AA0h,0AF0h,0B40h,0B90h,0BE0h,0C30h
                        dw 0C80h,0CD0h,0D20h,0D70h,0DC0h,0E10h,0E60h,0EB0h
                        dw 0F00h,0F50h,0FA0h,0FF0h,1040h,1090h,10E0h,1130h
                        dw 1180h,11D0h,1220h,1270h,12C0h,1310h,1360h,13B0h
                        dw 1400h,1450h,14A0h,14F0h,1540h,1590h,15E0h,1630h
                        dw 1680h,16D0h,1720h,1770h,17C0h,1810h,1860h,18B0h
                        dw 1900h,1950h,19A0h,19F0h,1A40h,1A90h,1AE0h,1B30h
                        dw 1B80h,1BD0h,1C20h,1C70h,1CC0h,1D10h,1D60h,1DB0h
                        dw 1E00h,1E50h,1EA0h,1EF0h,1F40h,1F90h,1FE0h,2030h
                        dw 2080h,20D0h,2120h,2170h,21C0h,2210h,2260h,22B0h
                        dw 2300h,2350h,23A0h,23F0h,2440h,2490h,24E0h,2530h
                        dw 2580h,25D0h,2620h,2670h,26C0h,2710h,2760h,27B0h
                        dw 2800h,2850h,28A0h,28F0h,2940h,2990h,29E0h,2A30h
                        dw 2A80h,2AD0h,2B20h,2B70h,2BC0h,2C10h,2C60h,2CB0h
                        dw 2D00h,2D50h,2DA0h,2DF0h,2E40h,2E90h,2EE0h,2F30h
                        dw 2F80h,2FD0h,3020h,3070h,30C0h,3110h,3160h,31B0h
                        dw 3200h,3250h,32A0h,32F0h,3340h,3390h,33E0h,3430h
                        dw 3480h,34D0h,3520h,3570h,35C0h,3610h,3660h,36B0h
                        dw 3700h,3750h,37A0h,37F0h,3840h,3890h,38E0h,3930h
                        dw 3980h,39D0h,3A20h,3A70h,3AC0h,3B10h,3B60h,3BB0h
                        dw 3C00h,3C50h,3CA0h,3CF0h,3D40h,3D90h,3DE0h,3E30h
                        dw 3E80h,3ED0h,3F20h,3F70h,3FC0h,4010h,4060h,40B0h
                        dw 4100h,4150h,41A0h,41F0h,4240h,4290h,42E0h,4330h
                        dw 4380h,43D0h,4420h,4470h,44C0h,4510h,4560h,45B0h
                        dw 4600h,4650h,46A0h,46F0h,4740h,4790h,47E0h,4830h
                        dw 4880h,48D0h,4920h,4970h,49C0h,4A10h,4A60h,4AB0h
                        dw 4B00h,4B50h,4BA0h,4BF0h,4C40h,4C90h,4CE0h,4D30h
                        dw 4D80h,4DD0h,4E20h,4E70h,4EC0h,4F10h,4F60h,4FB0h
                        dw 5000h,5050h,50A0h,50F0h,5140h,5190h,51E0h,5230h
                        dw 5280h,52D0h,5320h,5370h,53C0h,5410h,5460h,54B0h
                        dw 5500h,5550h,55A0h,55F0h,5640h,5690h,56E0h,5730h
                        dw 5780h,57D0h,5820h,5870h,58C0h,5910h,5960h,59B0h
                        dw 5A00h,5A50h,5AA0h,5AF0h,5B40h,5B90h,5BE0h,5C30h
                        dw 5C80h,5CD0h,5D20h,5D70h,5DC0h,5E10h,5E60h,5EB0h
                        dw 5F00h,5F50h,5FA0h,5FF0h,6040h,6090h,60E0h,6130h
                        dw 6180h,61D0h,6220h,6270h,62C0h,6310h,6360h,63B0h
                        dw 6400h,6450h,64A0h,64F0h,6540h,6590h,65E0h,6630h
                        dw 6680h,66D0h,6720h,6770h,67C0h,6810h,6860h,68B0h
                        dw 6900h,6950h,69A0h,69F0h,6A40h,6A90h,6AE0h,6B30h
                        dw 6B80h,6BD0h,6C20h,6C70h,6CC0h,6D10h,6D60h,6DB0h
                        dw 6E00h,6E50h,6EA0h,6EF0h,6F40h,6F90h,6FE0h,7030h
                        dw 7080h,70D0h,7120h,7170h,71C0h,7210h,7260h,72B0h
                        dw 7300h,7350h,73A0h,73F0h,7440h,7490h,74E0h,7530h
                        dw 7580h,75D0h,7620h,7670h,76C0h,7710h,7760h,77B0h
                        dw 7800h,7850h,78A0h,78F0h,7940h,7990h,79E0h,7A30h
                        dw 7A80h,7AD0h,7B20h,7B70h,7BC0h,7C10h,7C60h,7CB0h
                        dw 7D00h,7D50h,7DA0h,7DF0h,7E40h,7E90h,7EE0h,7F30h
                        dw 7F80h,7FD0h,8020h,8070h,80C0h,8110h,8160h,81B0h
                        dw 8200h,8250h,82A0h,82F0h,8340h,8390h,83E0h,8430h
                        dw 8480h,84D0h,8520h,8570h,85C0h,8610h,8660h,86B0h
                        dw 8700h,8750h,87A0h,87F0h,8840h,8890h,88E0h,8930h
                        dw 8980h,89D0h,8A20h,8A70h,8AC0h,8B10h,8B60h,8BB0h
                        dw 8C00h,8C50h,8CA0h,8CF0h,8D40h,8D90h,8DE0h,8E30h
                        dw 8E80h,8ED0h,8F20h,8F70h,8FC0h,9010h,9060h,90B0h
                        dw 9100h,9150h,91A0h,91F0h,9240h,9290h,92E0h,9330h
                        dw 9380h,93D0h,9420h,9470h,94C0h,9510h,9560h,95B0h

                        LOCALS

InitHanDrv:             retf

GrMode:                 mov     ax,0010h
                        int     10h
                        mov     ax,cs
                        mov     es,ax
                        mov     dx,offset CS:ColorTable
                        mov     ax,1002h
                        int     10h
                        retf

TextMode:               mov     ax,0003h
                        int     10h
                        retf

ClrScr:                 push     di

                        mov      ax,0A000h
                        mov      es,ax
                        xor      di,di
                        xor      ax,ax
                        mov      cx,80*480/2
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

                        mov      ax,0A000h
                        mov      es,ax
                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,cs:[bx+offset MulTable500]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      cs:InverseF,1
                        je       @@Inverse

                        lodsb                         ; 1
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 2
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 3
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 4
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 5
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 6
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 7
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 8
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 9
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 10
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 11
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 12
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 13
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 14
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 15
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 16
                        mov      es:[bx],al

                        pop      ds
                        pop      si
                        pop      bp
                        retf

       @@Inverse:       lodsb                         ; 1
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 2
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 3
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 4
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 5
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 6
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 7
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 8
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 9
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 10
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 11
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 12
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 13
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 14
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
                        lodsb                         ; 15
                        xor      al,0FFh
                        mov      es:[bx],al
                        add      bx,80
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
                        mov      ax,[bx+offset MulTable500]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      InverseF,1
                        mov      ax,0A000h
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
                        mov      ax,[bx+offset MulTable500]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      InverseF,1
                        mov      ax,0A000h
                        mov      es,ax
                        mov      ds,word ptr [HanFontPtr+2]
                        jne      HanNormal
                        jmp      HanInverse

             HanNormal: lodsw                         ; 1
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 3
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 4
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 5
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 6
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 7
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 8
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 9
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 10
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 11
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 12
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 13
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 14
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 15
                        or       ax,[di]
                        add      di,2
                        mov      es:[bx],ax
                        add      bx,80
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
                        add      bx,80
                        lodsw                         ; 2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 3
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 4
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 5
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 6
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 7
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 8
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 9
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 10
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 11
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 12
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 13
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 14
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 15
                        or       ax,[di]
                        add      di,2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
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
                        mov      ax,cs:[bx+offset MulTable500]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      cs:InverseF,0
                        mov      ax,0A000h
                        mov      es,ax
                        je       DLNormal
                        jmp      near ptr DLInverse

           DLNormal:    lodsw                         ; 1
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 2
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 3
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 4
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 5
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 6
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 7
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 8
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 9
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 10
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 11
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 12
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 13
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 14
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 15
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 16
                        mov      es:[bx],ax

                        pop      ds
                        pop      si
                        pop      bp
                        retf

         DLInverse:     lodsw                         ; 1
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 2
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 3
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 4
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 5
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 6
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 7
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 8
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 9
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 10
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 11
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 12
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 13
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 14
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
                        lodsw                         ; 15
                        xor      ax,0FFFFh
                        mov      es:[bx],ax
                        add      bx,80
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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
                        mov      ax,0A000h
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

ega640                  Ends

                        End      StartUp
