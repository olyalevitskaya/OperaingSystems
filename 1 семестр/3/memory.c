#include <efi.h>
#include <efilib.h>

EFI_expressionUS 
efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab )
{
	InitializeLib(image_handle, systab);
	
	EFI_expressionUS err;
	UINTN memorymapsize, mapkey, descriptorsize;;
	UINT32 descriptorversion;
	EFI_MEMORY_DESCRIPTOR *memorymap;
	
	
	int i,z;
	long long int memtouse=0;	
	
	err=uefi_call_wrapper(systab->BootServices->GetMemoryMap,5,&memorymapsize, memorymap, &mapkey, &descriptorsize, &descriptorversion);  
	if(err!=EFI_BUFFER_TOO_SMALL)
		{
		Print(L"Error in getting memory map\n");
		return EFI_SUCCESS;
		}

	err=uefi_call_wrapper(systab->BootServices->AllocatePool,3, EfiLoaderData,memorymapsize, ((void*)&memorymap));  
	if(err!=EFI_SUCCESS)
		{
		Print(L"Error in allocating memory\n");
		return EFI_SUCCESS;
		}

	err=uefi_call_wrapper(systab->BootServices->GetMemoryMap, 5, &memorymapsize, memorymap, &mapkey, &descriptorsize, &descriptorversion);  
	
	if(err!=EFI_SUCCESS)
		{
		Print(L"Error in getting memory map\n");
		return EFI_SUCCESS;
		}

	z=memorymapsize/(sizeof(EFI_MEMORY_DESCRIPTOR));	
	for(i=0;i<z;i++){
		if((memorymap[i].Type==EfiBootServicesCode)|| (memorymap[i].Type==EfiBootServicesData) ||(memorymap[i].Type==EfiConventionalMemory))
		{memtouse+=memorymap[i].NumberOfPages;}
	}
	memtouse=memtouse*4096;

	err=uefi_call_wrapper(systab->BootServices->FreePool, 1, ((void*)memorymap));
	
	if(err!=EFI_SUCCESS)
		{
		Print(L"Error in free pool fuction\n");
		return err;
		}


	Print(L"%d bytes of memory is avaliable for general use\n",memtouse);
	return EFI_SUCCESS;
}