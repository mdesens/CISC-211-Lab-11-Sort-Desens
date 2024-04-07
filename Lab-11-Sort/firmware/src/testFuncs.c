/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <malloc.h>
#include <inttypes.h>   // required to print out pointers using PRIXPTR macro
#include "definitions.h"                // SYS function prototypes
#include "projGlobalDefinitions.h" // lab test structs
#include "testFuncs.h" // lab test structs
#include "printFuncs.h"  // lab print funcs


static uint8_t txBuffer[MAX_PRINT_LEN] = {0};


static char * pass = "PASS";
static char * fail = "FAIL";
static char * oops = "OOPS";

// externs defined in the assembly file:


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */

// Returns num elements in array. Returns -1 if exceeds max allowed len,
// or invalid size
// Excludes trailing null. When searching for the null, it
// only examines the specified number of low-order bytes for 0, and
// ignores the other bytes
static int32_t getLength(void *pResult,int32_t elementSize)
{
//    uint8_t *bPtr = (uint8_t *)pResult;    // byte ptr
//    uint16_t *hwPtr = (uint16_t *)pResult; // half-word ptr
    uint32_t *wPtr = (uint32_t *)pResult;  // word ptr
    int32_t len = 0;
//    bool done = false;
    
    uint32_t val;
    do
    {
        val = *wPtr++;
        if (elementSize == 1)
        {
            val &= 0xFF;
        }
        else if (elementSize == 2)
        {
            val &= 0xFFFF;
        }
        if (val != 0)
        {
            len += 1;
            if (len > MAX_SORT_LEN) // too long, signal an error
            {
                len = -1;
                break;
            }
            continue; // otherwise, keep looking for null
        }
        // otherwise, must've been 0, time to leave the loop
        break;
    } while (true);
    
    return len;
}


// returns non-zero if there was a failure, whether forced or real
static int32_t check(int32_t in1, 
        int32_t in2, 
        int32_t *goodCount, 
        int32_t *badCount,
        bool forceFail, // if true, forces current test to fail
        char **pfString )
{
    int32_t retVal = 0;
    if ((forceFail == true) || (in1 != in2))
    {
        *badCount += 1;
        *pfString = fail;  
        retVal = 1;
    }
    else  //   (in1 == in2)
    {
        *goodCount += 1;
        *pfString = pass;
        retVal = 0;
    }    
    return retVal;
}

// =========   mySwap() ============================
// compares values stored in lowest bytes of two adjacent word32s.
// compares the number of bytes in each word specified in elementSize.
// Interprets those bytes as signed or unsigned according to "sign" parameter.
// Ignores higher order bytes. E.g. elementSize == 1 compares only byte 0
// in two adjacent 32 bit mem words.
// Swaps if bytes in lower mem location are > bytes in upper mem location
// Swaps ONLY the bytes specified. leaves other bytes in the word32
// untouched.
// Swapped IN-PLACE! If you want to keep pre-swap state, copy them
// before calling this func!
static int32_t mySwap(void * inpPtr, // swappable input values stored here
            int32_t sign, int32_t elementSize)
{
    // copy the input values to the temp output variables
    uint32_t expected_v1_postswap = *(int32_t *)inpPtr;
    uint32_t expected_v2_postswap = *((int32_t *)inpPtr+1);
    
    int32_t mySwapResult = 0;
    if(sign == 0) // process unsigned values
    {
        if (elementSize == 1) // unsigned bytes
        {
            uint8_t inpVal[2];
            inpVal[0] = *((uint8_t *) inpPtr);
            inpVal[1] = *((uint8_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                expected_v1_postswap &= 0xFFFFFF00;
                expected_v1_postswap |= inpVal[1];
                expected_v2_postswap &= 0xFFFFFF00;
                expected_v2_postswap |= inpVal[0];
            }
            else
            {
                mySwapResult = 0;
            }
        }
        else if (elementSize == 2) // unsigned halfwords
        {
            uint16_t inpVal[2];
            inpVal[0] = *((uint16_t *) inpPtr);
            inpVal[1] = *((uint16_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                expected_v1_postswap &= 0xFFFF0000;
                expected_v1_postswap |= inpVal[1];
                expected_v2_postswap &= 0xFFFF0000;
                expected_v2_postswap |= inpVal[0];
            }
            else
            {
                mySwapResult = 0;
            }
        }
        else if (elementSize == 4) // unsigned words
        {
            uint32_t inpVal[2];
            inpVal[0] = *((uint32_t *) inpPtr);
            inpVal[1] = *((uint32_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                expected_v1_postswap = inpVal[1];
                expected_v2_postswap = inpVal[0];
            }
            else
            {
                mySwapResult = 0;
            }
        }
    }
    else // process signed values
    {
        if (elementSize == 1) // signed bytes
        {
            int8_t inpVal[2];
            inpVal[0] = *((int8_t *) inpPtr);
            inpVal[1] = *((int8_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                // zero out the sign bits
                uint32_t t1 = inpVal[0];
                t1 &= 0xFF;
                uint32_t t2 = inpVal[1];
                t2 &= 0xFF;
                expected_v1_postswap &= 0xFFFFFF00;
                expected_v1_postswap |= t2;
                expected_v2_postswap &= 0xFFFFFF00;
                expected_v2_postswap |= t1;
            }
            else
            {
                mySwapResult = 0;
            }
        }
        else if (elementSize == 2) // signed halfwords
        {
            int16_t inpVal[2];
            inpVal[0] = *((int16_t *) inpPtr);
            inpVal[1] = *((int16_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                // zero out the sign bits
                uint32_t t1 = inpVal[0];
                t1 &= 0xFFFF;
                uint32_t t2 = inpVal[1];
                t2 &= 0xFFFF;
                expected_v1_postswap &= 0xFFFF0000;
                expected_v1_postswap |= t2;
                expected_v2_postswap &= 0xFFFF0000;
                expected_v2_postswap |= t1;
            }
            else
            {
                mySwapResult = 0;
            }
        }
        else if (elementSize == 4) // signed words
        {
            int32_t inpVal[2];
            inpVal[0] = *((int32_t *) inpPtr);
            inpVal[1] = *((int32_t *) (inpPtr+4));

            if((inpVal[0] == 0) || (inpVal[1]==0))
            {
                mySwapResult = -1;
            }
            else if (inpVal[0] > inpVal[1])
            {
                mySwapResult = 1;
                expected_v1_postswap = inpVal[1];
                expected_v2_postswap = inpVal[0];
            }
            else
            {
                mySwapResult = 0;
            }
        }
    }
    // copy the results back to the caller's mem locations
    *(int32_t *)inpPtr     = expected_v1_postswap;
    *((int32_t *)inpPtr+1) = expected_v2_postswap;
    //*expected_v1_postswap_ptr = expected_v1_postswap;
    //*expected_v2_postswap_ptr = expected_v2_postswap;
    return mySwapResult;
}


// bubble sort. Returns number of swaps.
// assumes length of input is valid, and array is null-terminated
static int32_t sortArray(void *inpArr, 
        int32_t sign, int32_t elementSize)
{
    uint32_t *u_wPtr = (uint32_t *)inpArr;  // unsigned word ptr
    int32_t numSwapsThisPass = 0;
    int32_t totalNumSwaps = 0;
    
    bool done = false;
    do
    {
        numSwapsThisPass = 0;
        int32_t swapVal = 0;
        do
        {
            numSwapsThisPass += swapVal;
            swapVal = mySwap((void  *)u_wPtr,
                    sign,
                    elementSize);
            u_wPtr++;  // incr pointer for next loop
        } while (swapVal != -1);
        if (numSwapsThisPass == 0)
        {
            done = true;
        }
        else 
        {
            totalNumSwaps += numSwapsThisPass;
            // reset pointer to start of the array
            u_wPtr = (uint32_t *)inpArr;
        }
    } while (done == false);
   
    return totalNumSwaps;
}



// return non-zero if arrays don't match
// always compare full arrays, don't stop at terminal
// null. Otherwise might miss student's code modifying
// bytes past the end of the null.

static int32_t compareArrays(int32_t *expArr,
        int32_t *testArr,
        int32_t *goodCount, 
        int32_t *badCount,
        bool forceFail, // if true, forces current test to fail
        char **pfString )
{
    int32_t mismatchCount = 0;
    for (int32_t i = 0; i < MAX_SORT_ARRAY_SIZE; ++i)
    {
        if (expArr[i] != testArr[i])
        {
            ++mismatchCount;
        }
    }
    if (mismatchCount == 0)
    {
        *goodCount += 1;
        *pfString = pass;
    }
    else
    {
        *badCount += 1;
        *pfString = fail;  
    }
    return mismatchCount;
}



// return -1 if invalid input
static int32_t calcExpectedValues(
        const void *inpArr, // unsorted input array from caller
        void *outArr, // pointer where caller wants sorted output to be stored
        int32_t sign, // input: 1 == signed, 0 == unsigned
        int32_t elementSize, // input: 1,2, or 4 byte elements
        expectedValues *e,
        volatile bool * txComplete) // in/out: ptr to struct where values will be stored
{

    // don't want to destroy original so copy input to another array that will be sorted

    for(int32_t i = 0;  i<MAX_SORT_ARRAY_SIZE; i++)
    {
        *(((uint32_t *)outArr) +i) = *(((uint32_t *)inpArr) +i);
    }
    e->expectedLen = getLength((void *)outArr,elementSize); // return num elements excluding trailing 0
    if(e->expectedLen < 0)
    {
        e->expectedSortedArr = (void *) 0;
        e->expectedNumSwaps = -1;
        return -1;
    }

    e->expectedSortedArr = outArr;
    /*
     * printArray1("calc exp vals, pre sort",
            (const uint32_t*)outArr,
            txComplete);
     * */
        
    // sort the values in the output array
    e->expectedNumSwaps = sortArray(outArr, sign, elementSize);
    /* printArray1("calc exp vals, post sort",
            (const uint32_t*)outArr,
            txComplete);
     * */
        
    if(e->expectedNumSwaps < 0)
    {
        return -1;
    }

    return 0;
}



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */


// function used to test functionality of student's asmSwap implementation
// 
// inpPtr points to a copy of the pair that was sent to asmFunc. The copy is
// unswapped.
// testPtr points to the mem location that contained the inp values sent to
// asmSwap. If asmSwap worked right, it should have tested and swapped
// the values in-place.
// The test checks for:
//   1) was a zero detected, or did a swap occur?
//   2) were the bytes containing the values correctly swapped based on their value
//   3) were the other bytes in the two mem locations untouched?
void testAsmSwap(int testNum, 
        char *desc, // optional description of test for printout
        const void * inpPtr, // start addr of test case input value pair
        void * testPtr, // start addr of pair after processed by asmSwap
        int32_t asmSwapResult, // -1, 0, or 1
        int32_t sign, // 1 ==  signed, 0 == unsigned
        int32_t elementSize,  // valid values 1,2,4 bytes
        int32_t* passCnt,
        int32_t* failCnt,
        volatile bool * txComplete)
{
    char * resCheck = oops;
    char * v1Check = oops;
    char * v2Check = oops;

    // make a copy of the values in the test case array so we don't mess that up    
    const int32_t * inp_i32Ptr = (const int32_t *)inpPtr;
    int32_t swappableCopy[2] = { *inp_i32Ptr,*(inp_i32Ptr+1) };
    
    // make a copy of the values in the test array so we don't mess that up    
    int32_t * test_i32Ptr = (int32_t *)testPtr;
    uint32_t inp_v1_preswap = *inp_i32Ptr;
    uint32_t inp_v2_preswap = *(inp_i32Ptr+1);
    
    int32_t mySwapResult = 0;
    
    // check the input test values, swap them if necessary
    mySwapResult = mySwap((void *)swappableCopy, // pre-swapped input values
            sign,elementSize);
    
    // compare returned result to value returned by asm
    check(mySwapResult,asmSwapResult,passCnt,failCnt,false,&resCheck);

    // compare our swapped values to the values returned by asm code
    check(swappableCopy[0], *test_i32Ptr,passCnt,failCnt,false,&v1Check);
    check(swappableCopy[1], *(test_i32Ptr+1),passCnt,failCnt,false,&v2Check);

    /*** uncomment to test printouts
    if (testNum == 20)
    {
        *failCnt = 42;
    }
     * ***/

    // STUDENTS: to print only failures, 
    // set ONLY_PRINT_SWAP_FAILS in prjGlobalDefinitions.h to true
    
    if((ONLY_PRINT_SWAP_FAILS == true && *failCnt != 0) ||
            (ONLY_PRINT_SWAP_FAILS == false))
    {
        snprintf((char*)txBuffer, MAX_PRINT_LEN,
            "========= %s Test Number: %d\r\n"
            "signed: %ld; element size: %ld\r\n"
            "Pre-swap input v1: 0x%08lx\r\n"
            "Pre-swap input v2: 0x%08lx\r\n"
            "%s: swap result expected:    %ld; actual: %ld\r\n"
            "%s: v1 expected: 0x%08lx; actual: 0x%08lx\r\n"
            "%s: v2 expected: 0x%08lx; actual: 0x%08lx\r\n"
            "tests passed: %ld; tests failed: %ld \r\n"
            "\r\n",
            desc,
            testNum,
            sign, elementSize,
            inp_v1_preswap, 
            inp_v2_preswap,
            resCheck,mySwapResult, asmSwapResult,
            v1Check,swappableCopy[0], *test_i32Ptr,
            v2Check,swappableCopy[1], *(test_i32Ptr+1),
            *passCnt,*failCnt); 

        printAndWait((char*)txBuffer,txComplete);
    }
    

    
    return;
}



// function used to test functionality of student's asmSort implementation
// 
void testAsmSort(int32_t testNum, int32_t sortArrIndex,
                char *desc, // optional description of test for printout
                void *inpArr, // unmodified copy of unsorted input array sent to asm
                // next ptr is where a copy of the unsorted test case input was made
                // The asm code is supposed to sort them in-place at this same location
                void * pResult,
                int32_t sign, // whether inp arr was signed or unsigned
                int32_t size, // num bytes for each element in array: 1,2, or 4
                int32_t numSwaps, // num swaps reported by asmSort to order the input array
                int32_t* passCnt,
                int32_t* failCnt,
                volatile bool * txComplete)
{    
    // store expected values here.
    expectedValues e;

    // declare an array that can hold the max num of largest-sized elements
    // this will be used to hold a sorted version of the test array.
    // This pointer will be saved in the expectedValues struct.
    uint32_t goodSortedArray[MAX_SORT_ARRAY_SIZE];

    // place to store pass/fail strings
    char * swapCheck = oops;
    char * lenCheck = oops;
    char * sortCheck = oops;

    *passCnt = 0;
    *failCnt = 0;
        
    int32_t err = calcExpectedValues(
            (const void *) inpArr,  // copy of unsorted array given to asm to sort
            (void *) goodSortedArray, // addr where this call will store sorted values
            sign,
            size, 
            &e,
            txComplete);
    
    // set force fail flag to false. If certain tests fail, 
    // remaining tests are forced to fail
    bool forceFail = false;
    
    // test to see if sorted array len matches expectations
    int32_t asmLength = -1;
    // return num elements excluding trailing 0. 
    // -1 if exceeds max allowable length
    asmLength = getLength(pResult,size); // return num elements excluding trailing 0
    err = check(e.expectedLen,asmLength,passCnt,failCnt,forceFail,&lenCheck );
    if (err != 0)
    {
        // force remaining tests to fail cuz can't compare different len arrays
        forceFail = true;
    }

    check(e.expectedNumSwaps,numSwaps,passCnt,failCnt,forceFail,&swapCheck);
    
    compareArrays(e.expectedSortedArr,
            pResult,
            passCnt,
            failCnt,
            forceFail,
            &sortCheck );


    /*** uncomment to test printouts
    if (testNum == 20)
    {
        *failCnt = 42;
    }
     * ***/
    
    if (*failCnt != 0)
    {
        printArray3(testNum,
            "pre-sort, c-sort, asm-sort (*** is sort mismatch)",
            (const uint32_t*)inpArr,
            (const uint32_t*)goodSortedArray,
            (const uint32_t*)pResult,            
            txComplete);
    }

    // STUDENTS: to print only failures, 
    // set ONLY_PRINT_SORT_FAILS in prjGlobalDefinitions.h to true
    
    if((ONLY_PRINT_SORT_FAILS == true && *failCnt != 0) ||
            (ONLY_PRINT_SORT_FAILS == false))
    {
        snprintf((char*)txBuffer, MAX_PRINT_LEN,
                "========= asmSort Test Number: %ld\r\n"
                "sort test case array index: %ld\r\n"
                "sign:                       %ld\r\n"
                "element size:               %ld\r\n"
                "%s: length expected:    %ld; actual: %ld\r\n"
                "%s: num swaps expected: %ld; actual: %ld\r\n"
                "%s: Result of sorted list comparison\r\n"
                "tests passed: %ld; tests failed: %ld \r\n"
                "\r\n",
                testNum,
                sortArrIndex,
                sign,
                size,
                lenCheck,e.expectedLen,asmLength,
                swapCheck,e.expectedNumSwaps,numSwaps,
                sortCheck,
                *passCnt,*failCnt); 

        printAndWait((char*)txBuffer,txComplete);
    }

    if (*failCnt != 0)
    {
        // this is really a no-op. It's intended to be a
        // convenient place to place a breakpoint on failures
        *txComplete = false;
    }
    
    return;
}




/* *****************************************************************************
 End of File
 */
