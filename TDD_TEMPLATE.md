/*
    adc_test.c
    Author:
       Maike Rodrigo A. Reis
       Electrical Engineering Stundent at University Federal of Pará
       maikerdralcantara@gmail.com
    Created on:
       10 Mar 2020
*/
//test harness include
#include "unity.h"

static void runAllTests(){
    RUN_TEST_GROUP(GroupName);
    RUN_TEST_GROUP(AnotherGroupName);
}

//You would define multiple Unity test main functions and build when you need to run 
//tests in chunks. You would choose to chunk tests when tests run too slowly 
//or when all tests will not fit into the memory of the target system
int main(int argc, char * argv[]){
    UnityMain(argc, argv, runAllTests)
}



//#includes for module under test

TEST_GROUP(GroupName);

//Define file scope data acessible to test group members prior to test_SETUP.

TEST_SETUO(GroupName){
    //initialization steps are executed before each TEST
}

TEST_TEAR_DOWN(GroupName){
    //clean up steps are executed after each TEST
}

TEST(GroupName, UniqueTesteName){
    /* 
        A Test contains:
            TEST specific initializations
            operations on the code under test
            TEST specific condition checks
    */
}

//There can be many tests in a TEST_GROUP
TEST(GroupName, AnotherUniqueTestName){
    /*
        Some more test statements
    */
}
//Each group has a TEST_GROUP_RUNNER
TEST_GROUP_RUNNER(GroupName){
    //Each TEST has a corresponding RUN_TEST_CASE
    RUN_TEST_CASE(GroupName, Unique)

}
