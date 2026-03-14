#include <iostream>
#include <VX/vx.h>
using namespace std;

int main()
{
	cout<<"This is the code to understatnd the vx_Scalar ooperation \n";
	vx_context cont = vxCreateContext();

	vx_float32 f32 =12132.342342;
	vx_scalar sca = vxCreateScalar(cont, VX_TYPE_FLOAT32, &f32 );

	vx_float32 f32_1;
	vxCopyScalar(sca, &f32_1, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] after vxCreateScalar f32_1 = " << f32_1 <<endl;

	vx_float32 f32_2 = 56.3434;
	vxCopyScalar(sca, &f32_2, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); // on write the value the scalar hold changes 
	// vxCopyScalar(sca, &f32_2, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] f32_2 =" <<f32_2<<endl; 
	cout<<"[DEBUG PRINT] f32_1 =" <<f32_1<<endl; // this is the proof that the value of the variable wont be changed only the value scalr hold changes on writes 
	cout<<"[DEBUG PRINT] f32 =" <<f32<<endl; // this is the proof that the value of the variable wont be changed only the value scalr hold changes on writes 

	vx_float32 f32_3 ;
	vxCopyScalar(sca, &f32_3, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] f32_3 =" <<f32_3<<endl; 


	vxReleaseScalar(&sca);
	vxReleaseContext(&cont);

}