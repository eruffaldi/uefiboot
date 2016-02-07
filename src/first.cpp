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
      ST->BootServices->SetWatchdogTimer(0, 0, 0, (CHAR16*) NULL);

      //Flashes cleans the console
      //Status = ST->ConOut->Reset(ST->ConOut, FALSE);
      /* Say hi */
      Status = ST->ConOut->OutputString(ST->ConOut, (CHAR16*) L"Hello World!!\n\r");
      //if (EFI_ERROR(Status))
      //return Status;
   
      Status = ST->ConIn->Reset(ST->ConIn, FALSE);
      if (EFI_ERROR(Status))
      {
          ST->ConOut->OutputString(ST->ConOut, (CHAR16*) L"Reset Key failed\n\r");
          return Status;
      }
      return Status;
    
      // test for multicore
      void * MpProto = 0;
      EFI_GUID mpg = EFI_MP_SERVICES_PROTOCOL_GUID;
      Status = ST->BootServices->LocateProtocol( &mpg, NULL, &MpProto);
      if(Status == EFI_SUCCESS)
      {
          ST->ConOut->OutputString(ST->ConOut, (CHAR16*) L"Has MP\n\r");        
      }

      //while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ; // polling from OSDEV example
      Status = ST->BootServices->WaitForEvent(1, &ST->ConIn->WaitForKey, &index);
      if (EFI_ERROR(Status))
      {
          ST->ConOut->OutputString(ST->ConOut, (CHAR16*) L"Wait Key failed\n\r");
          return Status;        
      }
      else
      {
          ST->ConOut->OutputString(ST->ConOut,(CHAR16*) L"Wait Key succeded\n\r");
          while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ; // polling from OSDEV example
      }
   
      return EFI_SUCCESS;
  }
}