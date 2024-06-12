//---------------------------------------------------------------------------
#ifndef registry_funcsH
#define registry_funcsH

#define pHKR  (( HKEY ) (ULONG_PTR)((LONG)0x80000000) )
#define pHKCU (( HKEY ) (ULONG_PTR)((LONG)0x80000001) )
#define pHKLM (( HKEY ) (ULONG_PTR)((LONG)0x80000002) )
#define pHKU  (( HKEY ) (ULONG_PTR)((LONG)0x80000003) )

#define HKR  "HKR"
#define HKU  "HKU"
#define HKLM "HKLM"
#define HKCU "HKCU"

#include <string>
#include <list>
#include <registry.hpp>
#include <bitset.h>
#include <sstream>


namespace reg
{
   list<std::string> getSubkeys(std::string rootKey, std::string Key);
   byte* toREG_BINARY(std::string Value);
   bool Write_REG_BINARY(std::string rootKey, std::string Key, std::string paramName, std::string HEX_as_Dec);
   std::string ReadString(std::string rootKey, std::string Key, std::string paramName, std::string retIfNExists);
   std::string ReadBinary(std::string rootKey, std::string Key, std::string paramName, std::string retIfNExists);
   bool WriteString(std::string rootKey, std::string Key, std::string paramName, std::string Value);
   std::string ReadInt(std::string rootKey, std::string Key, std::string DWORD_ParamName, std::string retIfNExists);
   bool WriteInt(std::string rootKey, std::string Key, std::string DWORD_ParamName, int DWORD_Value);
   bool ParamExists(std::string rootKey, std::string Key, std::string paramName);
   HKEY toHKEY(std::string hKey);
   std::string bitsetToString(bitset<8> Bitset);
   bitset<8> stringToBitset(std::string sBitset);
};


#endif
