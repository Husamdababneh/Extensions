#pragma once


#define DLLExport __declspec(dllexport) 


#define CScopeStart extern "C" {
#define CScopeEnd }
CScopeStart

#define parmeters(...) __VA_ARGS__

#define newFunc(x, returnType, ...) typedef returnType (_##x)(__VA_ARGS__);\
									 DLLExport returnType\
									 x(__VA_ARGS__);

newFunc(init, void,
	parmeters(void));

newFunc(update, int,
	parmeters(int, int));

newFunc(destroy, void, 
	parmeters(void));
// @TODO: Add new Functions.
CScopeEnd