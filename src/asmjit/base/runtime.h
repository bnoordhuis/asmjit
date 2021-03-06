// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Guard]
#ifndef _ASMJIT_BASE_RUNTIME_H
#define _ASMJIT_BASE_RUNTIME_H

// [Dependencies - AsmJit]
#include "../base/error.h"
#include "../base/vmem.h"

// [Api-Begin]
#include "../apibegin.h"

namespace asmjit {

//! @addtogroup asmjit_base_codegen
//! @{

// ============================================================================
// [Forward Declarations]
// ============================================================================

struct BaseAssembler;
struct BaseCpuInfo;

// ============================================================================
// [asmjit::Runtime]
// ============================================================================

//! Base runtime.
struct Runtime {
  ASMJIT_NO_COPY(Runtime)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `Runtime` instance.
  ASMJIT_API Runtime();
  //! Destroy the `Runtime` instance.
  ASMJIT_API virtual ~Runtime();

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  //! Get stack alignment of target runtime.
  virtual uint32_t getStackAlignment() = 0;

  //! Get CPU information.
  virtual const BaseCpuInfo* getCpuInfo() = 0;

  //! Allocate a memory needed for a code generated by `BaseAssembler` and
  //! relocate it to the target location.
  //!
  //! Returns Status code as `kError`.
  virtual Error add(void** dst, BaseAssembler* assembler) = 0;

  //! Release memory allocated by `add`.
  virtual Error release(void* p) = 0;
};

// ============================================================================
// [asmjit::JitRuntime]
// ============================================================================

//! JIT runtime.
struct JitRuntime : public Runtime {
  ASMJIT_NO_COPY(JitRuntime)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a `JitRuntime` instance.
  ASMJIT_API JitRuntime();
  //! Destroy the `JitRuntime` instance.
  ASMJIT_API virtual ~JitRuntime();

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  // Note: These members can be ignored by all derived classes. They are here
  // only to privide default implementation. All other implementations (remote
  // code patching or making dynamic loadable libraries/executables) ignore
  // members accessed by these accessors.

  //! Get the `VMemMgr` instance.
  ASMJIT_INLINE VMemMgr* getMemMgr() const {
    return const_cast<VMemMgr*>(&_memMgr);
  }

  //! Get the type of allocation.
  ASMJIT_INLINE uint32_t getAllocType() const {
    return _allocType;
  }

  //! Set the type of allocation.
  ASMJIT_INLINE void setAllocType(uint32_t allocType) {
    _allocType = allocType;
  }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  ASMJIT_API virtual uint32_t getStackAlignment();
  ASMJIT_API virtual const BaseCpuInfo* getCpuInfo();

  ASMJIT_API virtual Error add(void** dst, BaseAssembler* assembler);
  ASMJIT_API virtual Error release(void* p);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! Virtual memory manager.
  VMemMgr _memMgr;
  //! Type of allocation.
  uint32_t _allocType;
};

//! @}

} // asmjit namespace

// [Api-End]
#include "../apiend.h"

// [Guard]
#endif // _ASMJIT_BASE_RUNTIME_H
