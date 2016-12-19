        // Minimal c runtime initialization so that we can call gcc generate main
	// we rely on call to have setup the stack to a valid location
	// currently this is done by reset handler
	
	.text
.globl start
start:
	  la   gp, _gp                    // Initialize global pointer
	  la   t0, _fbss                  // clear bss
	  la   t1, _end

	  sd   zero, 0(t0)
	  addi t0, t0, 8
	
          mv   a0, zero                  // argc = 0
          mv   a1, zero                  // argv = NULL
          mv   a2, zero                   // envp = NULL
	
          jal  main

          sbreak

