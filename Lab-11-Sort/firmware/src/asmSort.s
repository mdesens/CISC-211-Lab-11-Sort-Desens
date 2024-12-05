/*** asmSort.s   ***/
#include <xc.h>
.syntax unified

@ Declare the following to be in data memory
.data
.align    

@ Define the globals so that the C code can access them
/* define and initialize global variables that C can access */
/* create a string */
.global nameStr
.type nameStr,%gnu_unique_object
    
/*** STUDENTS: Change the next line to your name!  **/
nameStr: .asciz "Mark Desens"  

.align   /* realign so that next mem allocations are on word boundaries */
 
/* initialize a global variable that C can access to print the nameStr */
.global nameStrPtr
.type nameStrPtr,%gnu_unique_object
nameStrPtr: .word nameStr   /* Assign the mem loc of nameStr to nameStrPtr */

@ Tell the assembler that what follows is in instruction memory    
.text
.align

/********************************************************************
function name: asmSwap(inpAddr,signed,elementSize)
function description:
    Checks magnitude of each of two input values 
    v1 and v2 that are stored in adjacent in 32bit memory words.
    v1 is located in memory location (inpAddr)
    v2 is located at mem location (inpAddr + M4 word size)
    
    If v1 or v2 is 0, this function immediately
    places -1 in r0 and returns to the caller.
    
    Else, if v1 <= v2, this function 
    does not modify memory, and returns 0 in r0. 

    Else, if v1 > v2, this function 
    swaps the values and returns 1 in r0

Inputs: r0: inpAddr: Address of v1 to be examined. 
	             Address of v2 is: inpAddr + M4 word size
	r1: signed: 1 indicates values are signed, 
	            0 indicates values are unsigned
	r2: size: number of bytes for each input value.
                  Valid values: 1, 2, 4
                  The values v1 and v2 are stored in
                  the least significant bits at locations
                  inpAddr and (inpAddr + M4 word size).
                  Any bits not used in the word may be
                  set to random values. They should be ignored
                  and must not be modified.
Outputs: r0 returns: -1 If either v1 or v2 is 0
                      0 If neither v1 or v2 is 0, 
                        and a swap WAS NOT made
                      1 If neither v1 or v2 is 0, 
                        and a swap WAS made             
             
         Memory: if v1>v2:
			swap v1 and v2.
                 Else, if v1 == 0 OR v2 == 0 OR if v1 <= v2:
			DO NOT swap values in memory.

NOTE: definitions: "greater than" means most positive number
********************************************************************/     
.global asmSwap
.type asmSwap,%function     
asmSwap:
    // save the caller registers, as required by the ARM calling convention
    push {r4-r11,LR}

    /* YOUR asmSwap CODE BELOW THIS LINE! VVVVVVVVVVVVVVVVVVVVV  */

    /* Load the first value from memory */
    ldr r4, [r0]       /* Load the value at address r0 into r4 */

    /* Load the second value from memory */
    ldr r5, [r0, 4]    /* Load the value at address r0 + 4 into r5 */

    /* Mask the values based on the size */
    cmp r2, 1
    beq mask_byte
    cmp r2, 2
    beq mask_halfword
    cmp r2, 4
    beq mask_word

mask_byte:
    and r4, r4, 0xFF   /* Mask the byte to get the value */
    and r5, r5, 0xFF   /* Mask the byte to get the value */
    b compare_values

mask_halfword:
    mov r6, 0xFFFF     /* Load the mask value into r6 */
    and r4, r4, r6     /* Mask the halfword to get the value */
    and r5, r5, r6     /* Mask the halfword to get the value */
    b compare_values

mask_word:
    /* No need to mask for word size */
    b compare_values
    
compare_values:
    /* Check if either v1 or v2 is 0 */
    cmp r4, 0
    beq set_return_negative_one
    cmp r5, 0
    beq set_return_negative_one

    /* Compare the values based on signed or unsigned */
    cmp r1, 0
    beq compare_unsigned

    /* Compare signed values */
    cmp r4, r5
    ble no_swap        /* If r4 <= r5, no swap is needed */
    b swap_elements

compare_unsigned:
    cmp r4, r5         /* Compare unsigned values */
    bls no_swap        /* If r4 <= r5, no swap is needed */

swap_elements:
    /* Swap the elements */
    cmp r2, 1          /* Check if the element size is 1 byte */
    beq swap_byte
    cmp r2, 2          /* Check if the element size is 2 bytes */
    beq swap_halfword
    cmp r2, 4          /* Check if the element size is 4 bytes */
    beq swap_word

swap_byte:
    strb r5, [r0]      /* Store r5 (next element) in the address r0 */
    strb r4, [r0, 4]   /* Store r4 (current element) in the address r0 + 4 */
    b swap_complete

swap_halfword:
    strh r5, [r0]      /* Store r5 (next element) in the address r0 */
    strh r4, [r0, 4]   /* Store r4 (current element) in the address r0 + 4 */
    b swap_complete

swap_word:
    str r5, [r0]       /* Store r5 (next element) in the address r0 */
    str r4, [r0, 4]    /* Store r4 (current element) in the address r0 + 4 */
    b swap_complete

swap_complete:
    /* If neither v1 nor v2 is 0, set return value to 1 (swap was made) */
    mov r0, 1
    b restore_registers

set_return_negative_one:
    /* Set return value to -1 if either v1 or v2 is 0 */
    mov r0, -1
    b restore_registers

no_swap:
    /* Set return value to 0 if neither v1 nor v2 is 0, and no swap was made */
    mov r0, 0

restore_registers:
    /* Restore the caller's registers, as required by the ARM calling convention */
    pop {r4-r11,LR}

    /* Return */
    mov pc, lr  /* asmSwap return to caller */

    
/********************************************************************
function name: asmSort(startAddr,signed,elementSize)
function description:
    Sorts value in an array from lowest to highest.
    The end of the input array is marked by a value
    of 0.
    The values are sorted "in-place" (i.e. upon returning
    to the caller, the first element of the sorted array 
    is located at the original startAddr)
    The function returns the total number of swaps that were
    required to put the array in order in r0. 
    
         
Inputs: r0: startAddr: address of first value in array.
		      Next element will be located at:
                          inpAddr + M4 word size
	r1: signed: 1 indicates values are signed, 
	            0 indicates values are unsigned
	r2: elementSize: number of bytes for each input value.
                          Valid values: 1, 2, 4
Outputs: r0: number of swaps required to sort the array
         Memory: The original input values will be
                 sorted and stored in memory starting
		 at mem location startAddr
NOTE: definitions: "greater than" means most positive number    
********************************************************************/     
.global asmSort
.type asmSort,%function
asmSort:   
    // save the caller registers, as required by the ARM calling convention
    push {r4-r11,LR}

    /* Note to Profs: 
     */

    /* Initialize total swap count to 0 */
    mov r4, 0          /* r4 will hold the total swap count */

outer_loop:
    /* Initialize inner swap count to 0 */
    mov r5, 0          /* r5 will hold the inner swap count */
    mov r6, r0         /* r6 will be used to traverse the array */

inner_loop:
    /* Load current element */
    ldr r7, [r6]       /* Load the value at address r6 into r7 */

    /* Load the next value from memory */
    add r8, r6, 4      /* Calculate the address of the next element */
    ldr r9, [r8]       /* Load the value at address r8 into r9 */

    /* Check if either value is 0 */
    cmp r7, 0
    beq complete_sort
    cmp r9, 0
    beq complete_sort

    /* Call asmSwap to swap the values if needed */
    mov r0, r6              /* Pass the address */
    mov r1, r1              /* Pass the signed flag (not required, but added in case of future code changes) */
    mov r2, r2              /* Pass the element size (same note as above) */
    push {r6, r7, r8, r9, r1, r2}   /* Save the registers */
    bl asmSwap                      /* Call asmSwap */
    pop {r6, r7, r8, r9, r1, r2}    /* Restore the registers */

    /* If a swap was made, increment the swap counts */
    cmp r0, 1
    bne check_loop_end         /* If no swap was made, continue to the next element */

increment_counts:
    add r4, r4, 1       /* Increment the total swap count */
    add r5, r5, 1       /* Increment the inner swap count */

check_loop_end:
    /* Check if we reached the end of the array */
    cmp r9, 0           /* Check if the next element is 0 */
    bne inner_loop      /* If not end of array, continue inner loop */

    /* If the inner swap count is 0, break out of the outer loop */
    cmp r5, 0 
    beq complete_sort

    /* Repeat the outer loop */
    b outer_loop

complete_sort:
    /* Store the total swap count in r0 */
    mov r0, r4

    /* Restore the caller's registers, as required by the ARM calling convention */
    pop {r4-r11,LR}

    /* Return */
    bx lr  /* asmSort return to caller */

/**********************************************************************/   
.end  /* The assembler will not process anything after this directive!!! */
           




