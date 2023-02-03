NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   0054   0040 =     64. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  __func_lit_start
       0094  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0094   006E =    110. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0094  __idata_start
       0102  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0102   2A0C =  10764. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0081  __text_start
       0081  __start
       00A3  _exit
       00A4  _init_devices
       00B3  _port_init
       00C2  _adc_init
       00C9  _timer0_init
       00D1  _timer1_init
       00DC  _timer2_init
       00E4  _uart0_init
       00EF  _adc_isr
       027A  _int0_isr
       0308  _int1_isr
       0323  _timer0_comp_isr
       0B87  _timer1_compa_isr
       0BD6  _timer2_comp_isr
       0BF6  _uart0_rx_isr
       0D90  _Progr
       0DC0  _Esc
       0DF2  _Vverh
       0E01  _Vniz
       0E10  _obrabotka_t
       1078  _Vvod
       1100  _main
       11E2  _transmit_data_1
       11F6  _transmit_data_2
       120E  _post2
       13A6  _post1
       1508  asr16
       150F  asr32
       151B  mod16s
       151F  div16s
       1539  mod16u
       153B  div16u
       153C  xdiv16u
       1555  pop_xgsetF0FC
       1560  push_xgsetF0FC
       1562  push_xgset30FC
       1564  push_xgset00FC
       1566  push_xgset003C
       156B  neg32
       1574  lsl32
       1580  lsr16
       1587  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   006E =    110. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       0060  _alfa
       0062  _alfa0
       0064  _alfamin
       0066  _alfamax
       0068  _k
       0069  _s
       006A  _o
       006B  _p
       006C  _r
       006D  _l
       006E  _oshibka_v
       0072  _oshibka_i
       0076  _oshibkaold_v
       007A  _oshibkaold_i
       007E  _Uvh
       0082  _Urn
       0086  _Urt
       008A  _Ui_p
       008E  _Ui_i
       0092  _Uout_v
       0096  _Uout_i
       009A  _Uv_i
       009E  _Uv_p
       00A2  _FWD
       00A3  _REV
       00A4  _FWD_ref
       00A5  _REV_ref
       00A6  _pause
       00A7  _standby
       00A8  _count
       00A9  _count1
       00AA  _fault
       00AB  _global
       00AC  _soft_start
       00AD  _t
       00AF  _sign_voltage
       00B0  _sign_current
       00B1  _cnt_i
       00B2  _alfa_i
       00B4  _flag_1
       00B5  _flag_2
       00B6  _post_1
       00B7  _post_2
       00B8  _temp
       00BA  _data
       00BB  _uart_data
       00BC  _flag1_1
       00BD  _flag1_2
       00BE  _kprop_v
       00C0  _tinteg_v
       00C2  _fkvant
       00C4  _kprop_i
       00C6  _tinteg_i
       00C8  _iupor
       00CA  _kos_v
       00CC  _kos_i
       00CE  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   00CE   005A =     90. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       00CE  __bss_start
       00CE  _volt_i
       00D0  _tok_i
       00D2  _voltage_i
       00FA  _current_i
       0122  _tok_os
       0124  _volt_os
       0126  _value
       0128  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0044 =     68. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
ildar.o	[ ildar.c ]
<library>	[ asr16.s, asr32.s, div16s.s, div16u.s, gpop5.s, gpush.s, lneg.s, lsl32.s, lsr16.s ]

User Global Definitions

ram_end = 0x85f
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:0.1024

