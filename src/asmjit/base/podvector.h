// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// Zlib - See LICENSE.md file in the package.

// [Guard]
#ifndef _ASMJIT_BASE_PODVECTOR_H
#define _ASMJIT_BASE_PODVECTOR_H

// [Dependencies - AsmJit]
#include "../base/defs.h"
#include "../base/error.h"
#include "../base/globals.h"

// [Api-Begin]
#include "../apibegin.h"

namespace asmjit {

//! @addtogroup asmjit_base_util
//! @{

// ============================================================================
// [asmjit::PodVectorData]
// ============================================================================

//! @internal
struct PodVectorData {
  //! Get data.
  ASMJIT_INLINE void* getData() const {
    return (void*)(this + 1);
  }

  //! Capacity of the vector.
  size_t capacity;
  //! Length of the vector.
  size_t length;
};

// ============================================================================
// [asmjit::PodVectorBase]
// ============================================================================

//! @internal
struct PodVectorBase {
  static ASMJIT_API const PodVectorData _nullData;

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a new instance of `PodVectorBase`.
  ASMJIT_INLINE PodVectorBase() :
    _d(const_cast<PodVectorData*>(&_nullData)) {}

  //! Destroy the `PodVectorBase` and data.
  ASMJIT_INLINE ~PodVectorBase() {
    if (_d != &_nullData)
      ::free(_d);
  }

  // --------------------------------------------------------------------------
  // [Grow / Reserve]
  // --------------------------------------------------------------------------

protected:
  ASMJIT_API Error _grow(size_t n, size_t sizeOfT);
  ASMJIT_API Error _reserve(size_t n, size_t sizeOfT);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

public:
  PodVectorData* _d;
};

// ============================================================================
// [asmjit::PodVector<T>]
// ============================================================================

//! Template used to store and manage array of POD data.
//!
//! This template has these adventages over other vector<> templates:
//! - Non-copyable (designed to be non-copyable, we want it)
//! - No copy-on-write (some implementations of stl can use it)
//! - Optimized for working only with POD types
//! - Uses ASMJIT_... memory management macros
template <typename T>
struct PodVector : PodVectorBase {
  ASMJIT_NO_COPY(PodVector<T>)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a new instance of `PodVector<T>`.
  ASMJIT_INLINE PodVector() {}
  //! Destroy the `PodVector<>` and data.
  ASMJIT_INLINE ~PodVector() {}

  // --------------------------------------------------------------------------
  // [Data]
  // --------------------------------------------------------------------------

  //! Get whether the vector is empty.
  ASMJIT_INLINE bool isEmpty() const {
    return _d->length == 0;
  }

  //! Get length.
  ASMJIT_INLINE size_t getLength() const {
    return _d->length;
  }

  //! Get capacity.
  ASMJIT_INLINE size_t getCapacity() const {
    return _d->capacity;
  }

  //! Get data.
  ASMJIT_INLINE T* getData() {
    return static_cast<T*>(_d->getData());
  }

  //! @overload
  ASMJIT_INLINE const T* getData() const {
    return static_cast<const T*>(_d->getData());
  }

  // --------------------------------------------------------------------------
  // [Clear / Reset]
  // --------------------------------------------------------------------------

  //! Clear vector data, but don't free an internal buffer.
  ASMJIT_INLINE void clear() {
    if (_d != &_nullData)
      _d->length = 0;
  }

  //! Clear vector data and free internal buffer.
  ASMJIT_INLINE void reset() {
    if (_d != &_nullData) {
      ::free(_d);
      _d = const_cast<PodVectorData*>(&_nullData);
    }
  }

  // --------------------------------------------------------------------------
  // [Grow / Reserve]
  // --------------------------------------------------------------------------

  //! Called to grow the buffer to fit at least `n` elements more.
  ASMJIT_INLINE Error _grow(size_t n) {
    return PodVectorBase::_grow(n, sizeof(T));
  }

  //! Realloc internal array to fit at least `n` items.
  ASMJIT_INLINE Error _reserve(size_t n) {
    return PodVectorBase::_reserve(n, sizeof(T));
  }

  // --------------------------------------------------------------------------
  // [Ops]
  // --------------------------------------------------------------------------

  //! Prepend `item` to vector.
  Error prepend(const T& item) {
    PodVectorData* d = _d;

    if (d->length == d->capacity) {
      ASMJIT_PROPAGATE_ERROR(_grow(1));
      _d = d;
    }

    ::memmove(static_cast<T*>(d->getData()) + 1, d->getData(), d->length * sizeof(T));
    ::memcpy(d->getData(), &item, sizeof(T));

    d->length++;
    return kErrorOk;
  }

  //! Insert an `item` at the `index`.
  Error insert(size_t index, const T& item) {
    PodVectorData* d = _d;
    ASMJIT_ASSERT(index <= d->length);

    if (d->length == d->capacity) {
      ASMJIT_PROPAGATE_ERROR(_grow(1));
      d = _d;
    }

    T* dst = static_cast<T*>(d->getData()) + index;
    ::memmove(dst + 1, dst, d->length - index);
    ::memcpy(dst, &item, sizeof(T));

    d->length++;
    return kErrorOk;
  }

  //! Append `item` to vector.
  Error append(const T& item) {
    PodVectorData* d = _d;

    if (d->length == d->capacity) {
      ASMJIT_PROPAGATE_ERROR(_grow(1));
      d = _d;
    }

    ::memcpy(static_cast<T*>(d->getData()) + d->length, &item, sizeof(T));

    d->length++;
    return kErrorOk;
  }

  //! Get index of `val` or `kInvalidIndex` if not found.
  size_t indexOf(const T& val) const {
    PodVectorData* d = _d;

    const T* data = static_cast<const T*>(d->getData());
    size_t len = d->length;

    for (size_t i = 0; i < len; i++)
      if (data[i] == val)
        return i;

    return kInvalidIndex;
  }

  //! Remove item at index `i`.
  void removeAt(size_t i) {
    PodVectorData* d = _d;
    ASMJIT_ASSERT(i < d->length);

    T* data = static_cast<T*>(d->getData()) + i;
    d->length--;
    ::memmove(data, data + 1, d->length - i);
  }

  //! Swap this pod-vector with `other`.
  void swap(PodVector<T>& other) {
    T* otherData = other._d;
    other._d = _d;
    _d = otherData;
  }

  //! Get item at index `i`.
  ASMJIT_INLINE T& operator[](size_t i) {
    ASMJIT_ASSERT(i < getLength());
    return getData()[i];
  }

  //! Get item at index `i`.
  ASMJIT_INLINE const T& operator[](size_t i) const {
    ASMJIT_ASSERT(i < getLength());
    return getData()[i];
  }

  //! Allocate and append a new item and return its address.
  T* newElement() {
    PodVectorData* d = _d;

    if (d->length == d->capacity) {
      if (!_grow(1))
        return NULL;
      d = _d;
    }

    return static_cast<T*>(d->getData()) + (d->length++);
  }
};

//! @}

} // asmjit namespace

// [Api-End]
#include "../apiend.h"

// [Guard]
#endif // _ASMJIT_BASE_PODVECTOR_H
