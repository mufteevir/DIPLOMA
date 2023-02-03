NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   0054   001E =     30. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  __func_lit_start
       0072  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0072   0143 =    323. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0072  __idata_start
       01B5  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   01B6   1BB0 =   7088. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       00DB  __text_start
       00DB  __start
       00FD  _exit
       00FE  _port_init
       0109  _timer0_init
       0111  _uart0_init
       011C  _timer0_comp_isr
       0190  _uart0_rx_isr
       021E  _post1
       04AD  _post2
       06A2  _transmit_data
       06AD  _data_ts
       0727  _digit
       07BA  _init_devices
       07C5  _main
       0808  _flag_2
       0A59  _flag_1
       0DFC  mod16u
       0DFE  div16u
       0DFF  xdiv16u
       0E18  div32u
       0E1A  mod32u
       0E1E  div32s
       0E20  mod32s
       0E4C  long_div_prolog
       0E5F  long_div_epilog
       0E6F  tst_R16_R19
       0E74  tst_R24_R27
       0E79  neg_R24_R27
       0E82  empy16s
       0E92  push_xgset300C
       0E97  pop_xgset300C
       0E9C  neg32
       0EA5  lsl16
       0EAC  lsr16
       0EB3  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   0143 =    323. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  __data_start
       0060  _data
       0061  _start_ts
       0062  _gain
       0063  _delay
       0064  _symbol
       0065  _stop_init
       0066  _flag
       0067  _flag1
       0068  _flag2
       0069  _cnt
       006A  _cnt1
       006C  _cnt2
       006E  _d
       006F  _dd
       0070  _ddd
       0071  _dddd
       0072  _alfa_i
       0074  _alfa_temp
       0076  _tok_i
       0078  _volt_i
       007A  _tok_temp
       007C  _volt_temp
       007E  _kprop_i_temp
       0080  _tinteg_i_temp
       0082  _global
       0084  _kprop_i
       0086  _tinteg_i
       0088  _kprop_v_temp
       008A  _tinteg_v_temp
       008C  _kprop_v
       008E  _tinteg_v
       0090  _kos_i_temp
       0092  _kos_v_temp
       0094  _kos_i
       0096  _kos_v
       0098  _fkvant_temp
       009A  _iupor_temp
       009C  _fkvant
       009E  _iupor
       00A0  _tmp
       00A1  _temp
       00A2  _perem
       00A3  _mass
       00DF  _mass1
       011D  _mass2
       013F  _mass3
       014D  _mass4
       0157  _mass5
       0161  _mass6
       016B  _mass7
       0175  _mass8
       017F  _mass9
       018F  _mass10
       019D  _uart_data
       019E  _post_1
       019F  _post_2
       01A0  _answer
       01A1  _f
       01A2  _y
       01A3  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   01A3   000A =     10. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       01A3  _power
       01A3  __bss_start
       01A7  _sign_power
       01A8  _sign_voltage
       01A9  _sign_current
       01AA  _i2
       01AB  _i1
       01AC  _i
       01AD  __bss_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0038 =     56. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
indicator.o	[ indicator.c ]
<library>	[ div16u.s, div32.s, emul16s.s, gpushx.s, lneg.s, lsl16.s, lsr16.s ]

User Global Definitions

ram_end = 0x85f
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:0.1024

