
Herc720                 Segment
                        Assume CS:Herc720,DS:Herc720

                        org    100h

StartUp:                int    20h

                        DW 89                    ; Max Width
                        DW 20                    ; Max Highth
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
       GrMode720_0      db 35h,2Dh,2Eh,07h,5Bh,02h,57h,57h,02h,03h,00h,00h,0Ah

       MulTable168      dw 0000h,0168h,02D0h,0438h,05A0h,0708h,0870h,09D8h
                        dw 0B40h,0CA8h,0E10h,0F78h,10E0h,1248h,13B0h,1518h
                        dw 1680h,17E8h,1950h,1AB8h,1C20h

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


       LineIndex        dw 0000h,2000h,4000h,6000h,005Ah,205Ah,405Ah,605Ah
                        dw 00B4h,20B4h,40B4h,60B4h,010Eh,210Eh,410Eh,610Eh
                        dw 0168h,2168h,4168h,6168h,01C2h,21C2h,41C2h,61C2h
                        dw 021Ch,221Ch,421Ch,621Ch,0276h,2276h,4276h,6276h
                        dw 02D0h,22D0h,42D0h,62D0h,032Ah,232Ah,432Ah,632Ah
                        dw 0384h,2384h,4384h,6384h,03DEh,23DEh,43DEh,63DEh
                        dw 0438h,2438h,4438h,6438h,0492h,2492h,4492h,6492h
                        dw 04ECh,24ECh,44ECh,64ECh,0546h,2546h,4546h,6546h
                        dw 05A0h,25A0h,45A0h,65A0h,05FAh,25FAh,45FAh,65FAh
                        dw 0654h,2654h,4654h,6654h,06AEh,26AEh,46AEh,66AEh
                        dw 0708h,2708h,4708h,6708h,0762h,2762h,4762h,6762h
                        dw 07BCh,27BCh,47BCh,67BCh,0816h,2816h,4816h,6816h
                        dw 0870h,2870h,4870h,6870h,08CAh,28CAh,48CAh,68CAh
                        dw 0924h,2924h,4924h,6924h,097Eh,297Eh,497Eh,697Eh
                        dw 09D8h,29D8h,49D8h,69D8h,0A32h,2A32h,4A32h,6A32h
                        dw 0A8Ch,2A8Ch,4A8Ch,6A8Ch,0AE6h,2AE6h,4AE6h,6AE6h
                        dw 0B40h,2B40h,4B40h,6B40h,0B9Ah,2B9Ah,4B9Ah,6B9Ah
                        dw 0BF4h,2BF4h,4BF4h,6BF4h,0C4Eh,2C4Eh,4C4Eh,6C4Eh
                        dw 0CA8h,2CA8h,4CA8h,6CA8h,0D02h,2D02h,4D02h,6D02h
                        dw 0D5Ch,2D5Ch,4D5Ch,6D5Ch,0DB6h,2DB6h,4DB6h,6DB6h
                        dw 0E10h,2E10h,4E10h,6E10h,0E6Ah,2E6Ah,4E6Ah,6E6Ah
                        dw 0EC4h,2EC4h,4EC4h,6EC4h,0F1Eh,2F1Eh,4F1Eh,6F1Eh
                        dw 0F78h,2F78h,4F78h,6F78h,0FD2h,2FD2h,4FD2h,6FD2h
                        dw 102Ch,302Ch,502Ch,702Ch,1086h,3086h,5086h,7086h
                        dw 10E0h,30E0h,50E0h,70E0h,113Ah,313Ah,513Ah,713Ah
                        dw 1194h,3194h,5194h,7194h,11EEh,31EEh,51EEh,71EEh
                        dw 1248h,3248h,5248h,7248h,12A2h,32A2h,52A2h,72A2h
                        dw 12FCh,32FCh,52FCh,72FCh,1356h,3356h,5356h,7356h
                        dw 13B0h,33B0h,53B0h,73B0h,140Ah,340Ah,540Ah,740Ah
                        dw 1464h,3464h,5464h,7464h,14BEh,34BEh,54BEh,74BEh
                        dw 1518h,3518h,5518h,7518h,1572h,3572h,5572h,7572h
                        dw 15CCh,35CCh,55CCh,75CCh,1626h,3626h,5626h,7626h
                        dw 1680h,3680h,5680h,7680h,16DAh,36DAh,56DAh,76DAh
                        dw 1734h,3734h,5734h,7734h,178Eh,378Eh,578Eh,778Eh
                        dw 17E8h,37E8h,57E8h,77E8h,1842h,3842h,5842h,7842h
                        dw 189Ch,389Ch,589Ch,789Ch,18F6h,38F6h,58F6h,78F6h
                        dw 1950h,3950h,5950h,7950h,19AAh,39AAh,59AAh,79AAh
                        dw 1A04h,3A04h,5A04h,7A04h,1A5Eh,3A5Eh,5A5Eh,7A5Eh
                        dw 1AB8h,3AB8h,5AB8h,7AB8h,1B12h,3B12h,5B12h,7B12h
                        dw 1B6Ch,3B6Ch,5B6Ch,7B6Ch,1BC6h,3BC6h,5BC6h,7BC6h
                        dw 1C20h,3C20h,5C20h,7C20h,1C7Ah,3C7Ah,5C7Ah,7C7Ah
                        dw 1CD4h,3CD4h,5CD4h,7CD4h,1D2Eh,3D2Eh,5D2Eh,7D2Eh
                        dw 1D88h,3D88h,5D88h,7D88h,1DE2h,3DE2h,5DE2h,7DE2h
                        dw 1E3Ch,3E3Ch,5E3Ch,7E3Ch

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
                        mov     si,offset GrMode720_0

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
                        shl      si,1
                        shl      si,1
                        shl      si,1
                        shl      si,1
                        add      si,word ptr cs:[EngFontPtr]

                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,cs:[bx+offset MulTable168]
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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

            @@Inverse:  lodsb                         ; 1
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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


DrawSpecial:
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        add      si,bx                ;  ds:si  font_pointer
                                                      ;  es:bx  screen pointer
                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,[bx+offset MulTable168]
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
                        shr      bl,1
                        shr      bl,1
                        mov      dl,byte ptr [bx+offset FirstIndex]
                        mov      bl,dl

                                 ;al  mid
                                 ;bl  first
                                 ;dh  tail

                        shl      bx,1
                        mov      si,word ptr [HanFontPtr]
                        add      si,[bx+HanMulTable]
                        mov      bl,al
                        shl      ax,1
                        shl      ax,1
                        shl      ax,1
                        shl      ax,1
                        shl      ax,1
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
                        shl      di,1
                        shl      di,1
                        shl      di,1
                        shl      di,1
                        shl      di,1
                        xor      bh,bh
                        shl      bx,1
                        add      di,word ptr [bx+ TailGroupIndex]
                        add      di,word ptr [HanFontPtr]
                        add      di,2*8

                        cmp      InverseF,1
                        mov      ax,0B000h
                        mov      es,ax
                        

                        mov      bx,word ptr [bp+8]
                        shl      bx,1
                        mov      ax,[bx+MulTable168]
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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

            HanInverse: lodsw                         ; 1
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        mov      ax,cs:[bx+offset MulTable168]
                        add      ax,word ptr [bp+6]
                        mov      bx,ax
                        cmp      cs:InverseF,0
                        mov      ax,0B000h
                        mov      es,ax
                        je       DLNormal
                        jmp      near ptr DLInverse

          DLNormal:     lodsw                         ; 1
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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

             DLInverse: lodsw                         ; 1
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        sub      bx,5FA6h
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        sub      bx,2
                        shl      word ptr [bp+8],1
                        shl      word ptr [bp+8],1
                        shl      word ptr [bp+8],1
                        shl      word ptr [bp+8],1
                        shl      word ptr [bp+8],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        shl      bx,1
                        inc      word ptr [bp+12]
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
                        shl      word ptr [bp+12],1
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

Herc720                 Ends

                        End      StartUp

