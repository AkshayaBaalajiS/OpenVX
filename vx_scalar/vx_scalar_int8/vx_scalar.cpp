#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the program to understand the vx_scalar of double type in cpp\n";
	vx_context cont = vxCreateContext();
	vx_int8 int8 ='a';
	vx_scalar scal = vxCreateScalar(cont, VX_TYPE_INT8, &int8);

	vx_int8 readSca;
	vxCopyScalar(scal, &readSca, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] readSca =  " << readSca <<endl;
	vxReleaseContext(&cont);
}