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

    /* YOUR asmSwap CODE BELOW THIS LINE! VVVVVVVVVVVVVVVVVVVVV  */

    /* Pseudocode for asmSwap function */
    /*
        Load the first value from memory
        Load the second value from memory
        If either value is 0, Return -1 (return value is in r0)

        Check if the first value is greater than the second value
        If the first value is not greater than the second value, Return 0

        Otherwise, Swap the values in memory using the steps below:
        Store the second value in a temporary location
        Store the first value in the location of the second value (using the correct size)
        Store the second value in the location of the first value (using the correct size)
        Return 1 to indicate a swap was made
    */

    /* YOUR asmSwap CODE ABOVE THIS LINE! ^^^^^^^^^^^^^^^^^^^^^  */
    
    
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
    cmp r2, 1          /* Check if the element size is 1 byte */
    beq load_byte
    cmp r2, 2          /* Check if the element size is 2 bytes */
    beq load_halfword
    cmp r2, 4          /* Check if the element size is 4 bytes */
    beq load_word

    
load_byte:
    ldr r7, [r6], 4    /* Load byte from address in r6 to r7 and increment r6 */
    and r7, r7, 0xFF   /* Mask the byte to get the value */
    ldr r8, [r6]       /* Load next byte from address in r6 to r8 */
    and r8, r8, 0xFF   /* Mask the byte to get the value */
    b compare_elements

load_halfword:
    ldr r7, [r6], 4    /* Load halfword from address in r6 to r7 and increment r6 */
    mov r9, 0xFFFF     /* Load the mask value into r9 */
    and r7, r7, r9     /* Mask the halfword to get the value */
    ldr r8, [r6]       /* Load next 32-bit word from address in r6 to r8 */
    and r8, r8, r9     /* Mask the halfword to get the value */
    b compare_elements

load_word:
    ldr r7, [r6], 4    /* Load word from address in r6 to r7 and increment r6 */
    ldr r8, [r6]       /* Load next word from address in r6 to r8 */
    b compare_elements

compare_elements:
    /* Compare the current element with the next element */
    cmp r7, r8
    ble no_swap         /* If r7 <= r8, no swap is needed */

    /* Swap the elements */
    cmp r2, 1           /* Check if the element size is 1 byte */
    beq swap_byte
    cmp r2, 2           /* Check if the element size is 2 bytes */
    beq swap_halfword
    cmp r2, 4           /* Check if the element size is 4 bytes */
    beq swap_word

swap_byte:

    strb r8, [r6, -4]   /* Store r8 (next element) in the previous address */
    strb r7, [r6]       /* Store r7 (current element) in the current address */
    b increment_counts

swap_halfword:
    strh r8, [r6, -4]   /* Store r8 (next element) in the previous address */
    strh r7, [r6]       /* Store r7 (current element) in the current address */
    b increment_counts

swap_word:
    str r8, [r6, -4]    /* Store r8 (next element) in the previous address */
    str r7, [r6]        /* Store r7 (current element) in the current address */
    b increment_counts

increment_counts:
    add r4, r4, 1       /* Increment the total swap count */
    add r5, r5, 1       /* Increment the inner swap count */

no_swap:
    /* Check if we reached the end of the array */
    cmp r8, 0           /* Check if the next element is 0 */
    bne inner_loop      /* If not end of array, continue inner loop */

    /* If the inner swap count is 0, break out of the outer loop */
    cmp r5, 0
    beq sorted

    /* Repeat the outer loop */
    b outer_loop

sorted:
    /* Store the total swap count in r0 */
    mov r0, r4

    /* Return */
    bx lr

    /* YOUR asmSort CODE ABOVE THIS LINE! ^^^^^^^^^^^^^^^^^^^^^  */

   

/**********************************************************************/   
.end  /* The assembler will not process anything after this directive!!! */
           




