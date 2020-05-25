#ifndef SHARED_H
#define SHARED_H

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "lex.h"

int GetNextTok(FILE	*file);
void MainLoop(FILE *file);

#endif // !SHARED_H

