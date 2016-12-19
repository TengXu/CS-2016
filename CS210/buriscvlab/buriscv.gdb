define sbreak
  b inst_fetch if  (m->state.cpu.core.regFile.gprs.reg[pc] == $arg0)
end
document sbreak
set a simulated break point on pc == $arg0
end

define sxbyte
  if $argc == 1
   set $sxbaddr = $arg0
   set $sxbcnt = 1
  end

  if $argc == 2
   set $sxbaddr = $arg0
   set $sxbcnt = $arg1
  end

  set $c = $sxbcnt
  set $d = 0

  while $c > 0
    set $lb = m->state.memory[$sxbaddr]
    set $c = $c - 1
    
    if $d == 0
      printf "0x%08x: 0x%02x (%c)", $sxbaddr, $lb, $lb
      set $d = $d + 1
    else
      printf " 0x%02x (%c)", $lb, $lb
      if $d == 3 
         printf "\n"
         set $d = 0
      else
         set $d = $d + 1
      end
    end
    set $sxbaddr = $sxbaddr + 1
  end
  printf "\n"
end
document sxbyte
print byte value of simulated memory at address arg0
end

define sxword
  if $argc == 1
   set $sxwaddr = $arg0
   set $sxwcnt = 1
  end

  if $argc == 2
   set $sxwaddr = $arg0
   set $sxwcnt = $arg1
  end

  set $c = $sxwcnt
  set $d = 0

  while $c > 0
    set $lw = *((Word *)&(m->state.memory[$sxwaddr]))
    set $c = $c - 1
    
    if $d == 0
      printf "0x%08x: 0x%08x ", $sxwaddr, $lw
      set $d = $d + 1
    else
      printf " 0x%08x ", $lw
      if $d == 3 
         printf "\n"
         set $d = 0
      else
         set $d = $d + 1
      end
    end
    set $sxwaddr = $sxwaddr + sizeof(Word)
  end
  printf "\n"
end
document sxword
print word value of simulated memory at address arg0
end

define saddr
  if $argc == 1
    printf "%p -> %p\n",$arg0,&(m->state.memory[$arg0])
  end
end
document saddr
  simulated memory address arg0 to underlying memory address of the simulation process
end 

define sreg
   if $argc == 1
     printf "%s (%s): 0x%08x\n", gprsNames[$arg0].gprName, gprsNames[$arg0].ccName, m->state.cpu.core.regFile.gprs.reg[$arg0]
   end
end
document sreg
print simulated register arg0
end


define ssp
  set $c = 1
  if $argc == 1
    set $c = $arg0
  end
  sxw m->state.cpu.core.regFile.gprs.reg[x2]  $c
end
document ssp
print top $arg0 simulated stack words 
end

define strace
  b inst_fetch
end
document strace
  set breakpoint for single stepping simulated instructions
end

define sdisassemble

  if $argc == 0
    set $sdaddr = m->state.cpu.core.regFile.gprs.reg[pc]
    set $sdcnt = 1
  end
  if $argc == 1
   set $sdaddr = $arg0
   set $sdcnt = 1
  end

  if $argc == 2
   set $sdaddr = $arg0
   set $sdcnt = $arg1
  end

  set $buf = (uint32_t *)&(m->state.memory[$sdaddr])
  call disassemble($sdaddr, $buf, $sdcnt)
  set $sdaddr = $sdaddr + $sdcnt

end
document sdisassemble
Disassemble simulated memory. Defaults to instruction at current pc
or you can specify a simulated address and optionally a count of instructions 
to disassemble eg. sdis 0x208 3
end 

define sstep
  c
  sreg pc
end
document sstep
  single step simulated instruction
end
   
