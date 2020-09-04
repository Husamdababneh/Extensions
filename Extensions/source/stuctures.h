#pragma once
#include <Windows.h>
#include <extensions_functions.h>

struct library {
	_init* init;
	_update* update;
	_destroy* destroy;
	HMODULE library;
};
