#include "stdafx.h"
#include "VectorTransform.h"
#include "MatlabFunctions.h"
#include "MatrixTransform.h"

using namespace std;

double koef_of_regr(double* buffer, double* etalon, int usenoise, int col, int len);