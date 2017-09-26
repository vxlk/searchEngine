#pragma once

#include "stringSet.h"

int main(void) {

	Stringset stringset;
	stringset.readFile();
	stringset.pageRank();
	stringset.print();
	int v;
	std::cin >> v;

	return 0;
}
