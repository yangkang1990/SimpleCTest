#SimpleCUnit Test#
smpUnit is unit testing framework for Embedded C System. It's design was copied from CUnit and more, and then remove some not applicable area， so it was simple to use. and you can use it in many system.In order to improve the program compatibility， this framework has Isolated the std c function. 


##how to use it##
 You can add unit tests to existing code to ensure that it works as you suspect.you also can study it to master common unittest framework。 then you can know the cunit easily 。 
 
 You unit tests are an investment. They let you to change yourcode and add new features confidently without worrying about accidentally breaking earlier features.


##getting start##
To add this simple cunit to your C code the only files you need are `smpUnit.c` `smpUnit.h` `smpUtil.c` `smpUtil.h`. others c source is the example of how to use the smpUnit to test you code.

if you want to see the test result, you can run make in the source directory.
then you run the file in exes directory like this:

	./exes/smpUnit




Author:yangkang   
Email:yangkang-1990@163.com   
Project Introduce:   
