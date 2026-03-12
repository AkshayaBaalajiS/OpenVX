#include <iostream>
#include <VX/vx.h>

using namespace std;

vx_status VX_CALLBACK customKernelFunction(vx_node node, const vx_reference * param, vx_uint32 num)
{
	cout<<"[DEBUG PRINT] customKernelFunction called on callback \n";
	vx_status stat = VX_SUCCESS;
	vx_image iimg = (vx_image)param[0];
	vx_image oimg = (vx_image)param[1];
	return stat;
}

vx_status VX_CALLBACK customKernelValidator(
    vx_node node,
    const vx_reference parameters[],
    vx_uint32 num,
    vx_meta_format metas[]
)
{
    vx_image input = (vx_image)parameters[0];

    vx_uint32 width;
    vx_uint32 height;
    vx_df_image format;

    vxQueryImage(input, VX_IMAGE_WIDTH, &width, sizeof(width));
    vxQueryImage(input, VX_IMAGE_HEIGHT, &height, sizeof(height));
    vxQueryImage(input, VX_IMAGE_FORMAT, &format, sizeof(format));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_WIDTH,
        &width,
        sizeof(width));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_HEIGHT,
        &height,
        sizeof(height));

    vxSetMetaFormatAttribute(
        metas[1],
        VX_IMAGE_FORMAT,
        &format,
        sizeof(format));

    return VX_SUCCESS;
}


int main()
{
	cout<<"This is the program to understand the vx_kernel creation \n";
	
	vx_context cont = vxCreateContext();

	vx_graph gra =vxCreateGraph(cont);

	vx_image iimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);
	vx_image oimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U8);

	// Query image 
	cout<<"---QUERY IMAGE--------\n";
	vx_uint32 width;
	vx_uint32 height;
	vxQueryImage(iimg, VX_IMAGE_WIDTH, &width, sizeof(width));
	vxQueryImage(iimg, VX_IMAGE_HEIGHT, &height, sizeof(height));
	cout<<"[DEBUG PRINT] width = "<< width <<std::endl;
	cout<<"[DEBUG PRINT] height = "<< height <<std::endl;
	cout<<"------------------------\n";

	// Image Patch 
	cout<<"-----IMAGE PATCH---------\n";
	vx_rectangle_t rect;
	rect.start_x =  0;
	rect.start_y =  0;
	rect.end_x =  640;
	rect.end_y =  640;

	vx_map_id map_id ;
	vx_imagepatch_addressing_t addr;
	void * ptr ;
	vx_status stat = vxMapImagePatch(iimg, &rect, 0, &map_id, &addr, &ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST, 0);
	if(stat == VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxMapImagePatch success\n";
	}
	cout<<"------------------------\n";


	vx_node nodeObj = vxGaussian3x3Node(gra, iimg, oimg);

	cout<<"------------------------\n";
	// 2. Register Kernel Function
	vx_kernel cust_kernel = vxAddUserKernel(cont, "user.CustomKernel", 8980, customKernelFunction, 2, customKernelValidator, NULL, NULL);
	// 3. Add parameter to Kernel
	vxAddParameterToKernel(cust_kernel, 0, VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED);
	vxAddParameterToKernel(cust_kernel, 1, VX_OUTPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED );
	// 4. Finalize kernel
	vxFinalizeKernel(cust_kernel);
	// 5. Create Node 
	vx_kernel createCustKern = vxGetKernelByName(cont, "user.CustomKernel");
	vx_node cust_Obj = vxCreateGenericNode(gra, createCustKern);
	vx_image iiimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U16);
	vx_image ooimg = vxCreateImage(cont, 640, 640, VX_DF_IMAGE_U16); 
	vxSetParameterByIndex(cust_Obj, 0, (vx_reference)iiimg);
	vxSetParameterByIndex(cust_Obj, 1, (vx_reference)ooimg);
	cout<<"------------------------\n";

	stat = vxVerifyGraph(gra);
	if(stat==VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxVerifyGraph failed \n";
		return -1;
	}
	stat = vxProcessGraph(gra);
	if(stat==VX_SUCCESS)
	{
		cout<<"[DEBUG PRINT] vxProcessGraph success\n";
	}
	else
	{
		cout<<"[DEBUG PRINT] vxProcessGraph failed \n";
		return -1;
	}
		

	vxUnmapImagePatch(iimg, map_id);

	vxReleaseImage(&iimg);
	vxReleaseImage(&oimg);
	vxReleaseImage(&iiimg);
	vxReleaseImage(&ooimg);
	

	vxReleaseGraph(&gra);

	vxReleaseContext(&cont);
}