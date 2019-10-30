#include <napi.h>
#include <stdio.h>
#include <vector>
#include <string>
extern "C" {
  #include "xed/xed-interface.h"
}

namespace node_xed {

  xed_state_t *xed_state = nullptr;
  void init_xed() {
    if (xed_state == nullptr)
    {
      xed_tables_init();

      xed_state = new xed_state_t();

      xed_state_zero(xed_state);
      xed_set_verbosity(99);

      xed_state_init2(xed_state,
        XED_MACHINE_MODE_LONG_64,
        XED_ADDRESS_WIDTH_64b);
    }
  }

  Napi::Array disassemble(Napi::Env env, Napi::Uint8Array assemblies, xed_uint64_t addr, char *addrFormat) {
    Napi::Array result = Napi::Array::New(env);

    init_xed();

    xed_uint_t   length = assemblies.ElementLength();

    #define BUFLEN  1000
    char buffer[BUFLEN];

    xed_uint_t offset = 0;

    xed_decoded_inst_t xdi;
    xed_uint_t xdi_len = 0;

    xed_uint_t index = 0;
    while (offset < length)
    {
      auto insn = Napi::Object::New(env);
      xed_decoded_inst_zero_set_mode(&xdi, xed_state);

      xed_error_enum_t error = xed_decode(&xdi, &assemblies[offset], length - offset);
      if (error == XED_ERROR_NONE)
      {
        xdi_len = xed_decoded_inst_get_length(&xdi);
        xed_uint_t ok = xed_format_context(XED_SYNTAX_INTEL, &xdi, buffer, BUFLEN, addr, 0, 0);
        if (ok) {
          insn.Set("asm", buffer);
        } else {
          buffer[0] = '\0';
          for (xed_uint_t i=0; i<xdi_len; ++i) {
            char tmp[100];
            sprintf(tmp, "%02Xh ", assemblies[offset+i]);
          }
          insn.Set("asm", buffer);
        }
      }
      else
      {
        xdi_len = 1;
        sprintf(buffer, "%02Xh", assemblies[offset]);
        insn.Set("asm", buffer);
      }

      sprintf(buffer, addrFormat, static_cast<long long unsigned int>(addr));
      insn.Set("addr", buffer);

      insn.Set("offset", offset);
      insn.Set("len", Napi::Number::New(env, xdi_len));

      result[index++] = insn;

      offset += xdi_len;
      addr += xdi_len;
    }

    return result;
  }

  Napi::Array disassembleWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    Napi::Uint8Array assemblies = info[0].As<Napi::TypedArray>().As<Napi::Uint8Array>();

    xed_uint64_t addr = 0;
    if (info.Length() >= 2) {
      if (info[1].IsString()) {
        std::string addrStr = info[1].As<Napi::String>();
        const char* cstr = addrStr.c_str();

        if (addrStr.size() >= 3 && cstr[0] == '0' && (cstr[1] == 'x' || cstr[1] == 'X')) {
          addr = strtoul(addrStr.c_str(), nullptr, 16);
        } else {
          addr = strtoul(addrStr.c_str(), nullptr, 10);
        }
      } else {
        addr = (xed_int64_t) info[1].As<Napi::Number>(); 
      }
    }

    char addrFormat[32];
    if (info.Length() >= 3) {
      int addrlen = (int) info[2].As<Napi::Number>();
      sprintf(addrFormat, "0x%%0%dllx", addrlen > 16 ? 16 : addrlen);
    } else {
      strcpy(addrFormat, "0x%llx");
    }

    return disassemble(env, assemblies, addr, addrFormat);
  }

  Napi::Object Init(Napi::Env env, Napi::Object exports) 
  {
    exports.Set("disassemble", Napi::Function::New(env, disassembleWrapped));
    return exports;
  }

  NODE_API_MODULE(xed, Init);
}
