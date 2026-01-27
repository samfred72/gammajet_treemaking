// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME gammajetDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
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
#include "cluster.h"
#include "jet.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *vectorlEjetgR_Dictionary();
   static void vectorlEjetgR_TClassManip(TClass*);
   static void *new_vectorlEjetgR(void *p = nullptr);
   static void *newArray_vectorlEjetgR(Long_t size, void *p);
   static void delete_vectorlEjetgR(void *p);
   static void deleteArray_vectorlEjetgR(void *p);
   static void destruct_vectorlEjetgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<jet>*)
   {
      vector<jet> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<jet>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<jet>", -2, "vector", 428,
                  typeid(vector<jet>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEjetgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<jet>) );
      instance.SetNew(&new_vectorlEjetgR);
      instance.SetNewArray(&newArray_vectorlEjetgR);
      instance.SetDelete(&delete_vectorlEjetgR);
      instance.SetDeleteArray(&deleteArray_vectorlEjetgR);
      instance.SetDestructor(&destruct_vectorlEjetgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<jet> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<jet>","std::vector<jet, std::allocator<jet> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<jet>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEjetgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<jet>*>(nullptr))->GetClass();
      vectorlEjetgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEjetgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEjetgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<jet> : new vector<jet>;
   }
   static void *newArray_vectorlEjetgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<jet>[nElements] : new vector<jet>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEjetgR(void *p) {
      delete (static_cast<vector<jet>*>(p));
   }
   static void deleteArray_vectorlEjetgR(void *p) {
      delete [] (static_cast<vector<jet>*>(p));
   }
   static void destruct_vectorlEjetgR(void *p) {
      typedef vector<jet> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<jet>

namespace ROOT {
   static TClass *vectorlEclustergR_Dictionary();
   static void vectorlEclustergR_TClassManip(TClass*);
   static void *new_vectorlEclustergR(void *p = nullptr);
   static void *newArray_vectorlEclustergR(Long_t size, void *p);
   static void delete_vectorlEclustergR(void *p);
   static void deleteArray_vectorlEclustergR(void *p);
   static void destruct_vectorlEclustergR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<cluster>*)
   {
      vector<cluster> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<cluster>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<cluster>", -2, "vector", 428,
                  typeid(vector<cluster>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEclustergR_Dictionary, isa_proxy, 4,
                  sizeof(vector<cluster>) );
      instance.SetNew(&new_vectorlEclustergR);
      instance.SetNewArray(&newArray_vectorlEclustergR);
      instance.SetDelete(&delete_vectorlEclustergR);
      instance.SetDeleteArray(&deleteArray_vectorlEclustergR);
      instance.SetDestructor(&destruct_vectorlEclustergR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<cluster> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<cluster>","std::vector<cluster, std::allocator<cluster> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<cluster>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEclustergR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<cluster>*>(nullptr))->GetClass();
      vectorlEclustergR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEclustergR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEclustergR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<cluster> : new vector<cluster>;
   }
   static void *newArray_vectorlEclustergR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<cluster>[nElements] : new vector<cluster>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEclustergR(void *p) {
      delete (static_cast<vector<cluster>*>(p));
   }
   static void deleteArray_vectorlEclustergR(void *p) {
      delete [] (static_cast<vector<cluster>*>(p));
   }
   static void destruct_vectorlEclustergR(void *p) {
      typedef vector<cluster> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<cluster>

namespace {
  void TriggerDictionaryInitialization_gammajetDict_Impl() {
    static const char* headers[] = {
"cluster.h",
"jet.h",
nullptr
    };
    static const char* includePaths[] = {
"/sphenix/user/samfred/projects/gammajet/install/include/",
"/cvmfs/sphenix.sdcc.bnl.gov/alma9.2-gcc-14.2.0/opt/sphenix/core/root-6.32.06/include/",
"/gpfs/mnt/gpfs02/sphenix/user/samfred/projects/gammajet/treemaking/src/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "gammajetDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$cluster.h")))  cluster;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$jet.h")))  jet;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "gammajetDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "cluster.h"
#include "jet.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("gammajetDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_gammajetDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_gammajetDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_gammajetDict() {
  TriggerDictionaryInitialization_gammajetDict_Impl();
}
