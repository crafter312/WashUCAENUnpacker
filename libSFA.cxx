// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libSFA
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

// Header files passed as explicit arguments
#include "src/CAENd5202.h"
#include "src/fiber.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *eventTiming_Dictionary();
   static void eventTiming_TClassManip(TClass*);
   static void *new_eventTiming(void *p = nullptr);
   static void *newArray_eventTiming(Long_t size, void *p);
   static void delete_eventTiming(void *p);
   static void deleteArray_eventTiming(void *p);
   static void destruct_eventTiming(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::eventTiming*)
   {
      ::eventTiming *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::eventTiming));
      static ::ROOT::TGenericClassInfo 
         instance("eventTiming", "src/CAENd5202.h", 12,
                  typeid(::eventTiming), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &eventTiming_Dictionary, isa_proxy, 4,
                  sizeof(::eventTiming) );
      instance.SetNew(&new_eventTiming);
      instance.SetNewArray(&newArray_eventTiming);
      instance.SetDelete(&delete_eventTiming);
      instance.SetDeleteArray(&deleteArray_eventTiming);
      instance.SetDestructor(&destruct_eventTiming);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::eventTiming*)
   {
      return GenerateInitInstanceLocal((::eventTiming*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::eventTiming*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *eventTiming_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::eventTiming*)nullptr)->GetClass();
      eventTiming_TClassManip(theClass);
   return theClass;
   }

   static void eventTiming_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","/home/Li6Webb/Desktop/SFA/caenUnpacker/src/CAENd5202.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Event_Dictionary();
   static void Event_TClassManip(TClass*);
   static void *new_Event(void *p = nullptr);
   static void *newArray_Event(Long_t size, void *p);
   static void delete_Event(void *p);
   static void deleteArray_Event(void *p);
   static void destruct_Event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Event*)
   {
      ::Event *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Event));
      static ::ROOT::TGenericClassInfo 
         instance("Event", "src/CAENd5202.h", 31,
                  typeid(::Event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Event_Dictionary, isa_proxy, 4,
                  sizeof(::Event) );
      instance.SetNew(&new_Event);
      instance.SetNewArray(&newArray_Event);
      instance.SetDelete(&delete_Event);
      instance.SetDeleteArray(&deleteArray_Event);
      instance.SetDestructor(&destruct_Event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Event*)
   {
      return GenerateInitInstanceLocal((::Event*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Event*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Event_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Event*)nullptr)->GetClass();
      Event_TClassManip(theClass);
   return theClass;
   }

   static void Event_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","/home/Li6Webb/Desktop/SFA/caenUnpacker/src/CAENd5202.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *fiber_Dictionary();
   static void fiber_TClassManip(TClass*);
   static void *new_fiber(void *p = nullptr);
   static void *newArray_fiber(Long_t size, void *p);
   static void delete_fiber(void *p);
   static void deleteArray_fiber(void *p);
   static void destruct_fiber(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::fiber*)
   {
      ::fiber *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::fiber));
      static ::ROOT::TGenericClassInfo 
         instance("fiber", "src/fiber.h", 10,
                  typeid(::fiber), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &fiber_Dictionary, isa_proxy, 4,
                  sizeof(::fiber) );
      instance.SetNew(&new_fiber);
      instance.SetNewArray(&newArray_fiber);
      instance.SetDelete(&delete_fiber);
      instance.SetDeleteArray(&deleteArray_fiber);
      instance.SetDestructor(&destruct_fiber);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::fiber*)
   {
      return GenerateInitInstanceLocal((::fiber*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::fiber*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *fiber_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::fiber*)nullptr)->GetClass();
      fiber_TClassManip(theClass);
   return theClass;
   }

   static void fiber_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_eventTiming(void *p) {
      return  p ? new(p) ::eventTiming : new ::eventTiming;
   }
   static void *newArray_eventTiming(Long_t nElements, void *p) {
      return p ? new(p) ::eventTiming[nElements] : new ::eventTiming[nElements];
   }
   // Wrapper around operator delete
   static void delete_eventTiming(void *p) {
      delete ((::eventTiming*)p);
   }
   static void deleteArray_eventTiming(void *p) {
      delete [] ((::eventTiming*)p);
   }
   static void destruct_eventTiming(void *p) {
      typedef ::eventTiming current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::eventTiming

namespace ROOT {
   // Wrappers around operator new
   static void *new_Event(void *p) {
      return  p ? new(p) ::Event : new ::Event;
   }
   static void *newArray_Event(Long_t nElements, void *p) {
      return p ? new(p) ::Event[nElements] : new ::Event[nElements];
   }
   // Wrapper around operator delete
   static void delete_Event(void *p) {
      delete ((::Event*)p);
   }
   static void deleteArray_Event(void *p) {
      delete [] ((::Event*)p);
   }
   static void destruct_Event(void *p) {
      typedef ::Event current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Event

namespace ROOT {
   // Wrappers around operator new
   static void *new_fiber(void *p) {
      return  p ? new(p) ::fiber : new ::fiber;
   }
   static void *newArray_fiber(Long_t nElements, void *p) {
      return p ? new(p) ::fiber[nElements] : new ::fiber[nElements];
   }
   // Wrapper around operator delete
   static void delete_fiber(void *p) {
      delete ((::fiber*)p);
   }
   static void deleteArray_fiber(void *p) {
      delete [] ((::fiber*)p);
   }
   static void destruct_fiber(void *p) {
      typedef ::fiber current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::fiber

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 339,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)nullptr)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEeventTiminggR_Dictionary();
   static void vectorlEeventTiminggR_TClassManip(TClass*);
   static void *new_vectorlEeventTiminggR(void *p = nullptr);
   static void *newArray_vectorlEeventTiminggR(Long_t size, void *p);
   static void delete_vectorlEeventTiminggR(void *p);
   static void deleteArray_vectorlEeventTiminggR(void *p);
   static void destruct_vectorlEeventTiminggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<eventTiming>*)
   {
      vector<eventTiming> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<eventTiming>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<eventTiming>", -2, "vector", 339,
                  typeid(vector<eventTiming>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEeventTiminggR_Dictionary, isa_proxy, 0,
                  sizeof(vector<eventTiming>) );
      instance.SetNew(&new_vectorlEeventTiminggR);
      instance.SetNewArray(&newArray_vectorlEeventTiminggR);
      instance.SetDelete(&delete_vectorlEeventTiminggR);
      instance.SetDeleteArray(&deleteArray_vectorlEeventTiminggR);
      instance.SetDestructor(&destruct_vectorlEeventTiminggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<eventTiming> >()));

      ::ROOT::AddClassAlternate("vector<eventTiming>","std::vector<eventTiming, std::allocator<eventTiming> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<eventTiming>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEeventTiminggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<eventTiming>*)nullptr)->GetClass();
      vectorlEeventTiminggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEeventTiminggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEeventTiminggR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<eventTiming> : new vector<eventTiming>;
   }
   static void *newArray_vectorlEeventTiminggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<eventTiming>[nElements] : new vector<eventTiming>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEeventTiminggR(void *p) {
      delete ((vector<eventTiming>*)p);
   }
   static void deleteArray_vectorlEeventTiminggR(void *p) {
      delete [] ((vector<eventTiming>*)p);
   }
   static void destruct_vectorlEeventTiminggR(void *p) {
      typedef vector<eventTiming> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<eventTiming>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 339,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)nullptr)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace {
  void TriggerDictionaryInitialization_libSFA_Impl() {
    static const char* headers[] = {
"src/CAENd5202.h",
"src/fiber.h",
nullptr
    };
    static const char* includePaths[] = {
"/usr/include/root",
"/home/Li6Webb/Desktop/SFA/caenUnpacker/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSFA dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(file_name@@@/home/Li6Webb/Desktop/SFA/caenUnpacker/src/CAENd5202.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$src/CAENd5202.h")))  eventTiming;
class __attribute__((annotate(R"ATTRDUMP(file_name@@@/home/Li6Webb/Desktop/SFA/caenUnpacker/src/CAENd5202.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$src/CAENd5202.h")))  Event;
class __attribute__((annotate("$clingAutoload$src/fiber.h")))  fiber;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSFA dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/CAENd5202.h"
#include "src/fiber.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"", payloadCode, "@",
"Event", payloadCode, "@",
"eventTiming", payloadCode, "@",
"fiber", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSFA",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSFA_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSFA_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSFA() {
  TriggerDictionaryInitialization_libSFA_Impl();
}
