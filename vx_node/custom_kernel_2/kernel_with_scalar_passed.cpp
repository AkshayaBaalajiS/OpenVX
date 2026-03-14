#include <iostream>
#include <VX/vx.h>
using namespace std;

vx_status VX_CALLBACK customKernelFunction(vx_node node, const vx_reference * para, vx_uint32 num)
{
	cout<<"--- [DEBUG PRINT]  VX_CALLBACK customKernelFunction --\n";
	vx_scalar isca = (vx_scalar)para[0];
	vx_scalar osca = (vx_scalar)para[1];

	vx_uint32 float32 ; 
	vx_status stat = vxCopyScalar(isca, &float32, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	stat = vxCopyScalar(osca, &float32, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);	

	return stat;
}
// vx_status VX_CALLBACK customKernelValidator(
//     vx_node node,
//     const vx_reference parameters[],
//     vx_uint32 num,
//     vx_meta_format metas[])
// {
//     return VX_SUCCESS;
// }
vx_status VX_CALLBACK customKernelValidator(
    vx_node node,
    const vx_reference parameters[],
    vx_uint32 num,
    vx_meta_format metas[])
{
    vx_scalar in = (vx_scalar)parameters[0];

    vx_enum type;

    vxQueryScalar(
        in,
        VX_SCALAR_TYPE,
        &type,
        sizeof(type));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_SCALAR_TYPE,
        &type,
        sizeof(type));

    return VX_SUCCESS;
}


int main()
{
	cout<<"This is the code to understand the Custom Kernel creation with scalr passed \n";

	vx_context cont = vxCreateContext();

	vx_graph gra = vxCreateGraph(cont);

	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);

	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);

	// Query Node 
	vx_uint32 width ;
	vx_uint32 height;
	vxQueryImage(iimg, VX_IMAGE_WIDTH, &width, sizeof(width) );
	vxQueryImage(oimg, VX_IMAGE_HEIGHT, &height, sizeof(height));
	cout<<"[DEBUG PRINT] width ==  "<<  width<<endl;
	cout<<"[DEBUG PRINT] height ==  "<<  height<<endl;
	
	// cust kernel 
	// 1. register kernel
	vx_kernel custKern  = vxAddUserKernel(cont, "user.custKernel", 8989, customKernelFunction, 2, customKernelValidator, NULL, NULL );
	// 2. Parameter to kernel 
	vxAddParameterToKernel(custKern, 0, VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED);
	vxAddParameterToKernel(custKern, 1, VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED);
	// 3. Finalize Kernel 
	vxFinalizeKernel(custKern); 
	// 4. Node Creation to Kernel 
	vx_kernel kenn = vxGetKernelByName(cont,"user.custKernel" );
	vx_uint32 uint32Obj = 12;
	vx_scalar isca = vxCreateScalar(cont, VX_TYPE_UINT32, &uint32Obj );
	vx_uint32 uint32Obj1 ;
	vx_scalar osca = vxCreateScalar(cont, VX_TYPE_UINT32, &uint32Obj1);
	vx_node custNode = vxCreateGenericNode(gra, kenn);
	vxSetParameterByIndex(custNode, 0, (vx_reference)isca);
	vxSetParameterByIndex(custNode, 1, (vx_reference)osca);

	vx_status stat =  vxVerifyGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success \n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed \n";
	}

	stat =  vxProcessGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success \n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed \n";
	}

	vxCopyScalar(osca,&uint32Obj1, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] output of Kernel is : "<< uint32Obj1<<endl;

	vxReleaseScalar(&isca);
	vxReleaseScalar(&osca);

	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}
