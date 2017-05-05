// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME RtoolDict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Rtool.hh"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_Rtool(void *p = 0);
   static void *newArray_Rtool(Long_t size, void *p);
   static void delete_Rtool(void *p);
   static void deleteArray_Rtool(void *p);
   static void destruct_Rtool(void *p);
   static void streamer_Rtool(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Rtool*)
   {
      ::Rtool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Rtool >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Rtool", ::Rtool::Class_Version(), "Rtool.hh", 96,
                  typeid(::Rtool), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Rtool::Dictionary, isa_proxy, 16,
                  sizeof(::Rtool) );
      instance.SetNew(&new_Rtool);
      instance.SetNewArray(&newArray_Rtool);
      instance.SetDelete(&delete_Rtool);
      instance.SetDeleteArray(&deleteArray_Rtool);
      instance.SetDestructor(&destruct_Rtool);
      instance.SetStreamerFunc(&streamer_Rtool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Rtool*)
   {
      return GenerateInitInstanceLocal((::Rtool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Rtool*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_TextField(void *p);
   static void deleteArray_TextField(void *p);
   static void destruct_TextField(void *p);
   static void streamer_TextField(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TextField*)
   {
      ::TextField *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TextField >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TextField", ::TextField::Class_Version(), "Rtool.hh", 40,
                  typeid(::TextField), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TextField::Dictionary, isa_proxy, 16,
                  sizeof(::TextField) );
      instance.SetDelete(&delete_TextField);
      instance.SetDeleteArray(&deleteArray_TextField);
      instance.SetDestructor(&destruct_TextField);
      instance.SetStreamerFunc(&streamer_TextField);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TextField*)
   {
      return GenerateInitInstanceLocal((::TextField*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TextField*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NumberFieldInt(void *p);
   static void deleteArray_NumberFieldInt(void *p);
   static void destruct_NumberFieldInt(void *p);
   static void streamer_NumberFieldInt(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NumberFieldInt*)
   {
      ::NumberFieldInt *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NumberFieldInt >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NumberFieldInt", ::NumberFieldInt::Class_Version(), "Rtool.hh", 59,
                  typeid(::NumberFieldInt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NumberFieldInt::Dictionary, isa_proxy, 16,
                  sizeof(::NumberFieldInt) );
      instance.SetDelete(&delete_NumberFieldInt);
      instance.SetDeleteArray(&deleteArray_NumberFieldInt);
      instance.SetDestructor(&destruct_NumberFieldInt);
      instance.SetStreamerFunc(&streamer_NumberFieldInt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NumberFieldInt*)
   {
      return GenerateInitInstanceLocal((::NumberFieldInt*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::NumberFieldInt*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_NumberFieldReal(void *p);
   static void deleteArray_NumberFieldReal(void *p);
   static void destruct_NumberFieldReal(void *p);
   static void streamer_NumberFieldReal(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NumberFieldReal*)
   {
      ::NumberFieldReal *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::NumberFieldReal >(0);
      static ::ROOT::TGenericClassInfo 
         instance("NumberFieldReal", ::NumberFieldReal::Class_Version(), "Rtool.hh", 77,
                  typeid(::NumberFieldReal), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::NumberFieldReal::Dictionary, isa_proxy, 16,
                  sizeof(::NumberFieldReal) );
      instance.SetDelete(&delete_NumberFieldReal);
      instance.SetDeleteArray(&deleteArray_NumberFieldReal);
      instance.SetDestructor(&destruct_NumberFieldReal);
      instance.SetStreamerFunc(&streamer_NumberFieldReal);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NumberFieldReal*)
   {
      return GenerateInitInstanceLocal((::NumberFieldReal*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::NumberFieldReal*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr Rtool::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Rtool::Class_Name()
{
   return "Rtool";
}

//______________________________________________________________________________
const char *Rtool::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Rtool*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Rtool::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Rtool*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Rtool::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Rtool*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Rtool::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Rtool*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TextField::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TextField::Class_Name()
{
   return "TextField";
}

//______________________________________________________________________________
const char *TextField::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TextField*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TextField::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TextField*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TextField::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TextField*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TextField::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TextField*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NumberFieldInt::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NumberFieldInt::Class_Name()
{
   return "NumberFieldInt";
}

//______________________________________________________________________________
const char *NumberFieldInt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldInt*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NumberFieldInt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldInt*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NumberFieldInt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldInt*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NumberFieldInt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldInt*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr NumberFieldReal::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *NumberFieldReal::Class_Name()
{
   return "NumberFieldReal";
}

//______________________________________________________________________________
const char *NumberFieldReal::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldReal*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int NumberFieldReal::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldReal*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *NumberFieldReal::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldReal*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *NumberFieldReal::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::NumberFieldReal*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void Rtool::Streamer(TBuffer &R__b)
{
   // Stream an object of class Rtool.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Rtool(void *p) {
      return  p ? new(p) ::Rtool : new ::Rtool;
   }
   static void *newArray_Rtool(Long_t nElements, void *p) {
      return p ? new(p) ::Rtool[nElements] : new ::Rtool[nElements];
   }
   // Wrapper around operator delete
   static void delete_Rtool(void *p) {
      delete ((::Rtool*)p);
   }
   static void deleteArray_Rtool(void *p) {
      delete [] ((::Rtool*)p);
   }
   static void destruct_Rtool(void *p) {
      typedef ::Rtool current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Rtool(TBuffer &buf, void *obj) {
      ((::Rtool*)obj)->::Rtool::Streamer(buf);
   }
} // end of namespace ROOT for class ::Rtool

//______________________________________________________________________________
void TextField::Streamer(TBuffer &R__b)
{
   // Stream an object of class TextField.

   TGHorizontalFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_TextField(void *p) {
      delete ((::TextField*)p);
   }
   static void deleteArray_TextField(void *p) {
      delete [] ((::TextField*)p);
   }
   static void destruct_TextField(void *p) {
      typedef ::TextField current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TextField(TBuffer &buf, void *obj) {
      ((::TextField*)obj)->::TextField::Streamer(buf);
   }
} // end of namespace ROOT for class ::TextField

//______________________________________________________________________________
void NumberFieldInt::Streamer(TBuffer &R__b)
{
   // Stream an object of class NumberFieldInt.

   TGHorizontalFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NumberFieldInt(void *p) {
      delete ((::NumberFieldInt*)p);
   }
   static void deleteArray_NumberFieldInt(void *p) {
      delete [] ((::NumberFieldInt*)p);
   }
   static void destruct_NumberFieldInt(void *p) {
      typedef ::NumberFieldInt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_NumberFieldInt(TBuffer &buf, void *obj) {
      ((::NumberFieldInt*)obj)->::NumberFieldInt::Streamer(buf);
   }
} // end of namespace ROOT for class ::NumberFieldInt

//______________________________________________________________________________
void NumberFieldReal::Streamer(TBuffer &R__b)
{
   // Stream an object of class NumberFieldReal.

   TGHorizontalFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NumberFieldReal(void *p) {
      delete ((::NumberFieldReal*)p);
   }
   static void deleteArray_NumberFieldReal(void *p) {
      delete [] ((::NumberFieldReal*)p);
   }
   static void destruct_NumberFieldReal(void *p) {
      typedef ::NumberFieldReal current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_NumberFieldReal(TBuffer &buf, void *obj) {
      ((::NumberFieldReal*)obj)->::NumberFieldReal::Streamer(buf);
   }
} // end of namespace ROOT for class ::NumberFieldReal

namespace {
  void TriggerDictionaryInitialization_RtoolDict_Impl() {
    static const char* headers[] = {
"Rtool.hh",
0
    };
    static const char* includePaths[] = {
"/usr/local/Cellar/root6/6.06.04/include/root",
"/Users/hasegawahiroaki/QAQC/NewRtool/NewRtool/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "RtoolDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Rtool.hh")))  Rtool;
class __attribute__((annotate("$clingAutoload$Rtool.hh")))  TextField;
class __attribute__((annotate("$clingAutoload$Rtool.hh")))  NumberFieldInt;
class __attribute__((annotate("$clingAutoload$Rtool.hh")))  NumberFieldReal;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "RtoolDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Rtool.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"NumberFieldInt", payloadCode, "@",
"NumberFieldReal", payloadCode, "@",
"Rtool", payloadCode, "@",
"TextField", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("RtoolDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_RtoolDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_RtoolDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_RtoolDict() {
  TriggerDictionaryInitialization_RtoolDict_Impl();
}
