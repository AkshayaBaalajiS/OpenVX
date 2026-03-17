#include <iostream>
#include <VX/vx.h>

using namespace std;

int main()
{
	cout<<"This is the program to understand the vx scalar of float type \n";
	vx_context cont = vxCreateContext();

	vx_float32 flt = 32.343544;
	vx_scalar scal = vxCreateScalar(cont, VX_TYPE_FLOAT32, &flt);

	vx_float32 oflt;
	vxCopyScalar(scal, &oflt, VX_READ_ONLY, VX_MEMORY_TYPE_HOST );
	cout<<"[DEBUG PRINT] oflt = " << oflt <<endl;
}