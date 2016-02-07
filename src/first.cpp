extern "C"
{
#include <efi.h>
#include <efilib.h>
#include <MPService.h>
}

extern "C"
{
  EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
  {
      UINTN index;
      EFI_STATUS Status;
      EFI_INPUT_KEY Key;
   
      /* Store the system table for future use in other functions */
      ST = SystemTable;
    
      /* Disable Watchdog */
      uefi_call_wrapper(ST->BootServices->SetWatchdogTimer,4,0, 0, 0, (CHAR16*) NULL);

      //Flashes cleans the console
      //Status = ST->ConOut->Reset(ST->ConOut, FALSE);
      /* Say hi */
      Status = uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut, (CHAR16*) L"Hello World!!\n\r");
      //if (EFI_ERROR(Status))
      //return Status;
   
      Status = uefi_call_wrapper(ST->ConIn->Reset,2,ST->ConIn, FALSE);
      if (EFI_ERROR(Status))
      {
          uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut, (CHAR16*) L"Reset Key failed\n\r");
          return Status;
      }
    
      // test for multicore
      void * MpProto = 0;
      EFI_GUID mpg = EFI_MP_SERVICES_PROTOCOL_GUID;
      Status = uefi_call_wrapper(ST->BootServices->LocateProtocol,3, &mpg, NULL, &MpProto);
      if(Status == EFI_SUCCESS)
      {
          uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut, (CHAR16*) L"Has MP\n\r");        
      }

      //while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ; // polling from OSDEV example
      Status = uefi_call_wrapper(ST->BootServices->WaitForEvent,3,1, &ST->ConIn->WaitForKey, &index);
      if (EFI_ERROR(Status))
      {
          uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut, (CHAR16*) L"Wait Key failed\n\r");
          return Status;        
      }
      else
      {
          uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut,(CHAR16*) L"Wait Key succeded\n\r");
          while ((Status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke,2,ST->ConIn, &Key)) == EFI_NOT_READY) ; // polling from OSDEV example
          uefi_call_wrapper(ST->ConOut->OutputString,2,ST->ConOut,(CHAR16*) L"Wait Key received\n\r");
      }
   
      return EFI_SUCCESS;
  }
}