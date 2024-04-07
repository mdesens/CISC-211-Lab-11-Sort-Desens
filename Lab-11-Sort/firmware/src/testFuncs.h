/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _TEST_FUNCS_H    /* Guard against multiple inclusion */
#define _TEST_FUNCS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
// #define EXAMPLE_CONSTANT 0

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
typedef struct _expectedValues
{
    int32_t expectedLen;
    int32_t expectedNumSwaps;
    void * expectedSortedArr;
    int32_t sign;
    int32_t size;
} expectedValues;

    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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


// function used to test functionality of student's asmSwap implementation
// 
void testAsmSwap(int testNum, 
        char *desc, // optional description of test for printout
        const void * v1,
        void * v2,
        int32_t swapResult,
        int32_t sign, // 1 ==  signed, 0 == unsigned
        int32_t elementSize,  // valid values 1,2,4 bytes
        int32_t* passCnt,
        int32_t* failCnt,
        volatile bool * txComplete);

//
//
//
//
// function used to test functionality of student's asmSort implementation
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
                volatile bool * txComplete);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _TEST_FUNCS_H */

/* *****************************************************************************
 End of File
 */
