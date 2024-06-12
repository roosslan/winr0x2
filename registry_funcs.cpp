//---------------------------------------------------------------------------
#pragma hdrstop
#include "registry_funcs.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace reg
{
   std::list<std::string> getSubkeys(std::string rootKey, std::string Key)
   {
     std::list<std::string> ret;
     TStringList *tsl = new TStringList();

     TRegistry *reg = new TRegistry(KEY_READ);
     try
     {
        reg->RootKey = toHKEY(rootKey);
        reg->OpenKeyReadOnly(Key.c_str());
        reg->GetKeyNames(tsl);
        for (int i = 0; i < tsl->Count ; ++i)
        {
           ret.push_back(tsl->Strings[i].c_str());
        }
        {
      }
      reg->CloseKey();
     }
     __finally
     {
        delete reg;
     }
     return ret;
   }

   // Конвертировать строку вида
   // 115, 0, 22, 12
   // в четыре HEX числа
byte* reg::toREG_BINARY(std::string Value)
   {
      byte buff[4];
      TStringList *tsl = new TStringList();
      TSysCharSet tws, delim;
      delim << ',';
      ExtractStrings(delim, tws, const_cast<char*>(Value.c_str()), tsl);
      for(int i = 0; i < 4; ++i)
      {
         AnsiString aStr = "0x" + Trim(tsl->Strings[i]);
	 buff[i] = strtol(aStr.c_str(), NULL, 16);
      }
      delete tsl;
      return buff;
   }

HKEY reg::toHKEY(std::string hKey)
{
   if (hKey == HKR)
   	return pHKR;

   if (hKey == HKU)
   	return pHKU;

   if (hKey == HKCU)
   	return pHKCU;

   if (hKey == HKLM)
   	return pHKLM;

   return NULL;
}

  std::string ReadBinary(std::string rootKey, std::string Key, std::string paramName, std::string retIfNExists)
  {
     AnsiString LValue;
     bitset<8> bitvec;
     byte buff[4];
     std::string ret;

     TRegistry *reg = new TRegistry(KEY_READ);
     try
     {
        reg->RootKey = toHKEY(rootKey);
        if(reg->OpenKeyReadOnly(Key.c_str()))
        {
           if(!reg->ValueExists(paramName.c_str()))
         	ret = retIfNExists; // "ParamNotExists"
           else
           {
	      reg->ReadBinaryData(paramName.c_str(), buff, sizeof(buff));

              for(int i = 0; i < 4; ++i)
              {
                 std::ostringstream oss;
                 oss << hex;
                 oss << (int)buff[i];

                 AnsiString aStr = oss.str().c_str();
                 LValue += aStr + ",";
              }
              ret = LValue.c_str();
	      ret.pop_back();
           }
      }
      reg->CloseKey();
     }
     __finally
     {
        delete reg;
     }
     return ret;
  }

  bool WriteBinary(std::string rootKey, std::string Key, std::string paramName, byte Value[4])
  {
    bool ret = false;
    TRegistry *reg = new TRegistry(KEY_WRITE);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      reg->OpenKey(Key.c_str(), true);
      reg->WriteBinaryData(paramName.c_str(), Value, 4);
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }
    
  bool Write_REG_BINARY(std::string rootKey, std::string Key, std::string paramName, std::string HEX_as_Dec)
  {
//     funcs *ffun = new funcs(NULL);

     byte *buff = toREG_BINARY(HEX_as_Dec);
     // WTF?:
     byte RValue[4];
     for(int i = 0; i < 4; ++i)
     {
       RValue[i] = buff[i];
     }
     WriteBinary(rootKey, Key, paramName, RValue);
//     delete ffun;
     return true;
  }

  bool ParamExists(std::string rootKey, std::string Key, std::string paramName)
  {
    bool ret = true;
    TRegDataType regtyp;
    TRegistry *reg = new TRegistry(KEY_READ);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      if (reg->OpenKey(Key.c_str(), false))
      {
         regtyp = reg->GetDataType(paramName.c_str());

         switch (regtyp)
         {
            case rdUnknown: ret = false;
            break;
            case rdString:
              if (reg->ReadString(paramName.c_str()) == NULL)
                ret = false;
            break;
            case rdInteger:
              if (reg->ReadInteger(paramName.c_str()) == NULL)
                ret = false;
            break;
            case rdBinary:
              byte *buff;
              if (reg->ReadBinaryData(paramName.c_str(), buff, sizeof(buff)) == NULL)
                ret = false;
            break;
           }
      }
      else ret = false;
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  std::string ReadString(std::string rootKey, std::string Key, std::string paramName, std::string retIfNExists)
  {
    std::string ret = retIfNExists;
    TRegistry *reg = new TRegistry(KEY_READ);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      if(reg->OpenKeyReadOnly(Key.c_str()))
      {
         if(!reg->ValueExists(paramName.c_str()))
         	ret = retIfNExists; // "ParamNotExists"
         else
         {
            ret = reg->ReadString(paramName.c_str()).c_str();
         }
      }
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  std::string ReadInt(std::string rootKey, std::string Key, std::string DWORD_ParamName, std::string retIfNExists)
  {
    std::string ret = retIfNExists;
    TRegistry *reg = new TRegistry(KEY_READ);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      if(reg->OpenKeyReadOnly(Key.c_str()))
      {
         if(!reg->ValueExists(DWORD_ParamName.c_str()))
         	ret = retIfNExists; // "ParamNotExists"
         else
         {
            ret = reg->ReadString(DWORD_ParamName.c_str()).c_str();
         }
      }
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  bool WriteInt(std::string rootKey, std::string Key, std::string DWORD_ParamName, int DWORD_Value)
  {
    bool ret = false;
    TRegistry *reg = new TRegistry(KEY_WRITE);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      reg->OpenKey(Key.c_str(), true);
      reg->WriteInteger(DWORD_ParamName.c_str(), DWORD_Value);
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  bool WriteString(std::string rootKey, std::string Key, std::string paramName, std::string Value)
  {
    bool ret = false;
    TRegistry *reg = new TRegistry(KEY_WRITE);
    try
    {
      reg->RootKey = toHKEY(rootKey);
      reg->OpenKey(Key.c_str(), true);
      reg->WriteString(paramName.c_str(), Value.c_str());
      reg->CloseKey();
    }
    __finally
    {
      delete reg;
    }
    return ret;
  }

  bitset<8> stringToBitset(std::string sBitset)
  {
     bitset<8> ret;
     for(byte i; i <= 4; ++i)
     {
        ret[i] = sBitset[i];
     }
     return ret;

  }

  std::string bitsetToString(bitset<8> Bitset)
  {
     // return Bitsec.to_string<std::string::value_type, std::string::traits_type, std::string::allocator_type >();
     return Bitset.template to_string<char, char_traits<char>, allocator<char> >();
  }

} // namespace