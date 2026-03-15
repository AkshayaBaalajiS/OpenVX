#include <iostream>
#include <VX/vx.h>
using namespace std;

// 1. Kernel call back creation 
vx_status VX_CALLBACK customKernelCallback(vx_node node, const vx_reference * ref, vx_uint32 num)
{
	cout<<"--------  Custom Kernel  ---------\n";
	vx_status stat = VX_SUCCESS;
	vx_scalar isca = vx_scalar(ref[0]);
	vx_scalar osca = vx_scalar(ref[1]);
	vx_uint32 iscal;
	vxCopyScalar(isca, &iscal, VX_READ_ONLY, VX_MEMORY_TYPE_HOST);
	cout<<"[DEBUG PRINT] in callback the input pass is : " << iscal <<endl;

	return stat;
	cout<<"----------------------------------\n";
}

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
	cout<<"This is the code to understand the OpenVX custom kernel read and write on Kernel \n";

	vx_context cont = vxCreateContext();
	if(cont!=nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateContext success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateContext failed \n";
	}

	vx_graph gra = vxCreateGraph(cont);
	if(gra!=nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateGraph failed \n";
	}

	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	// play with vx_image 
	vx_uint32 width;
	vx_uint32 height;
	vxQueryImage(iimg, VX_IMAGE_WIDTH,&width, sizeof(&width));
	vxQueryImage(iimg, VX_IMAGE_HEIGHT, &height, sizeof(&height));
	cout<<"[DEBUG PRINT] width = " << width << endl;
	cout<<"[DEBUG PRINT] height = " << height << endl;

	vx_rectangle_t rect;
	rect.start_x = 0;
	rect.start_y = 0;
	rect.end_x = 640;
	rect.end_y = 640;

	vx_imagepatch_addressing_t addr;
	vx_map_id map_id;
	void * ptr ;
	vxMapImagePatch(iimg, &rect, 0, &map_id, &addr, &ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0 );
	void * ptr1;
	vxMapImagePatch(iimg, &rect, 0, &map_id, &addr, &ptr1, VX_READ_ONLY, VX_MEMORY_TYPE_HOST, 0 );


	if(iimg==nullptr || oimg==nullptr )
	{
		cout<<"[DEBUG PRINT] vxCreateImage of input and output failed \n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxCreateImage of input and output success \n";
	}

	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);
	if(nodeObj!=nullptr)
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxGaussian3x3Node failed\n";
	}

	vx_uint32 iscal = 32;
	vx_scalar isca = vxCreateScalar(cont, VX_TYPE_UINT32, &iscal);
	if(isca == nullptr)
	{
		cout<<"[DEBUG PRINT] isca vxCreateScalar failed\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] isca vxCreateScalar success\n";
	}
	vx_uint32 oscal;
	vx_scalar oisca = vxCreateScalar(cont, VX_TYPE_UINT32, &oscal);
	if(oisca == nullptr)
	{
		cout<<"[DEBUG PRINT] oisca vxCreateScalar failed\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] oisca vxCreateScalar success\n";
	}


	// 2. callback registration 
	vx_kernel custKer = vxAddUserKernel(cont, "user.CustomKernel", 8989, customKernelCallback, 2, customKernelValidator, NULL, NULL);
	if(custKer == nullptr)
	{
		cout<<"[DEBUG PRINT] vxAddUserKernel failed\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxAddUserKernel success \n";
	}
	// 3. Add parameter to kernel 
	vxAddParameterToKernel(custKer, 0, VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED);
	vxAddParameterToKernel(custKer, 1, VX_OUTPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED);
	// 4. Finalize Kernel 
	vxFinalizeKernel(custKer);
	// 5. Get Kernel by name 
	vx_kernel kernel1 = vxGetKernelByName(cont, "user.CustomKernel");
	if(kernel1 == nullptr)
	{
		cout<<"[DEBUG PRINT] vxGetKernelByName failed\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxGetKernelByName success\n";
	}
	vx_node nodeObj1 = vxCreateGenericNode(gra, kernel1);
	if(nodeObj1 ==nullptr)
	{
		cout<<"[DEBUG PRINT] vxCreateGenericNode failed\n";
	}
	// 6. nOde parameyer 
	vxSetParameterByIndex(nodeObj1, 0, (vx_reference)isca);
	vxSetParameterByIndex(nodeObj1, 1, (vx_reference)oisca);
	cout<<"[DEBUG PRINT] vxCreateGenericNode creation success\n";

	vx_status stat = vxVerifyGraph(gra);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed\n";
	}




	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}